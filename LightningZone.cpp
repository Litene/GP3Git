// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningZone.h"

#include "GenericActor.h"
#include "ILightningConductorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LongLostLightning/EventInitiator.h"
#include "LongLostLightning/LongLostLightningCharacter.h"
#include "NiagaraComponent.h"

// Sets default values
ALightningZone::ALightningZone() {
	PrimaryActorTick.bCanEverTick = true;
	LightningZoneTrigger = CreateDefaultSubobject<UBoxComponent>("Lightning Zone");
	SetRootComponent(LightningZoneTrigger);
	UpTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("Up Time");
	DownTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("Down Time");

	UpInterpfunction.BindUFunction(this, FName("UpTimelineFloatReturn"));
	DownInterpfunction.BindUFunction(this, FName("DownTimelineFloatReturn"));

	UpInterpFinished.BindUFunction(this, FName("UpOnTimeLineFinished"));
	DownInterpFinished.BindUFunction(this, FName("DownOnTimeLineFinished"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->bAllowConcurrentTick = false;
}

void ALightningZone::SetCurrentTargetNotRandomized() {
	if (LastTickConductor && !LightningConductors.Contains(LastTickConductor)) {
		// Last tick isn't part of current frames conductors - Send lost top conductor event and nullify reference
		if (const auto GenericActor = Cast<AGenericActor>(LastTickConductor)) {
			GenericActor->Execute_OnLostTopConductor(GenericActor);
		}
		if (const auto Character = Cast<ALongLostLightningCharacter>(LastTickConductor)) {
			Character->Execute_OnLostTopConductor(Character);
		}

		LastTickTargetActor = nullptr;
		LastTickConductor = nullptr;
		CurrentTargetConductor = LightningConductors[0];
		CurrentTargetActor = LightningConductors[0]->ClassReference;
	}

	if (!LastTickTargetActor || !LastTickConductor) {
		// Last tick isn't valid
		CurrentTargetActor = LightningConductors[0]->ClassReference;
		CurrentTargetConductor = LightningConductors[0];
		LastTickTargetActor = CurrentTargetActor;
		LastTickConductor = CurrentTargetConductor;
	}

	if (LastTickConductor->Conductivity >= LightningConductors[0]->Conductivity) {
		// Same top conductivity value like last frame - no change of target
		CurrentTargetActor = LastTickTargetActor;
		CurrentTargetConductor = LastTickConductor;
	}

	else if (LightningConductors[0]->Conductivity > LastTickConductor->Conductivity) {
		// New top conductivity - change target
		CurrentTargetConductor = LightningConductors[0];
		CurrentTargetActor = LightningConductors[0]->ClassReference;
	}

	// On Lost Target
	if (LastTickConductor != CurrentTargetConductor) {
		if (const auto GenericActor = Cast<AGenericActor>(LastTickConductor)) {
			GenericActor->Execute_OnLostTopConductor(GenericActor);
		}
		if (const auto Character = Cast<ALongLostLightningCharacter>(LastTickConductor)) {
			Character->Execute_OnLostTopConductor(Character);
		}
	}
	
	LastTickTargetActor = CurrentTargetActor;
	LastTickConductor = CurrentTargetConductor;
}

void ALightningZone::OnLostTarget() {
	if (const auto GenericActor = Cast<AGenericActor>(LastTickConductor)) {
		GenericActor->Execute_OnLostTopConductor(GenericActor);
	}
	if (const auto Character = Cast<ALongLostLightningCharacter>(LastTickConductor)) {
		Character->Execute_OnLostTopConductor(Character);
	}
	OnNoTarget();
	CurrentTargetActor = nullptr;
	CurrentTargetConductor = nullptr;
	CurrentTargetLocation = FVector::ZeroVector;
}

void ALightningZone::UpTimelineFloatReturn(float Value) {
	if (LightningConductors.Num() == 0 && CurrentTargetActor) {
		OnLostTarget();
		return;
	}

	UpdateTimer += 0.01f;
	if (UpdateTimer > UpdateDateCooldown) {
		// Calculate and Sort Conductors
		CombinedConductors.Empty();
		UpdateTimer = 0;
		CleanConductors();
		if (LightningConductors.Num() == 0) {
			if (CurrentTargetActor) {
				OnLostTarget();
			}
			return;
		}
		for (const auto Conductor : LightningConductors) {
			if (const auto GenericActor = Cast<AGenericActor>(Conductor)) {
				GenericActor->CalculateConductivity();
			}
			if (const auto Character = Cast<ALongLostLightningCharacter>(Conductor)) {
				Character->CalculateConductivity();
			}
		}

		SortLightningConductors();
		OnConductorsCalculated(CurrentTargetActor);
		SetCurrentTargetNotRandomized();

		// Nearby conductors & average position
		if (CurrentTargetConductor->NearbyConductors.Num() > 0) {
			CombinedConductors.Add(Cast<AActor>(CurrentTargetActor));
			for (const auto Conductor : CurrentTargetConductor->NearbyConductors) {
				if (LightningConductors[0]->Conductivity == Conductor->Conductivity) {
					CombinedConductors.Add(Cast<AActor>(Conductor));
				}
			}
			CurrentTargetLocation = UGameplayStatics::GetActorArrayAverageLocation(CombinedConductors);
		}
		else {
			CurrentTargetLocation = CurrentTargetConductor->ClassReference->GetActorLocation();
		}
	}

	if (LightningConductors.Num() > 0 && CurrentTargetActor && LightningConductors.Contains(CurrentTargetConductor)) {
		OnLightningTick(CurrentTargetActor, 1 - Value);
	}
	else if (CurrentTargetActor && !LightningConductors.Contains(CurrentTargetConductor)) {
		// When we have the current target not in the list of local conductors we have lost our target
		OnLostTarget();
	}
}

void ALightningZone::DownTimelineFloatReturn(float Value) {
}

void ALightningZone::UpOnTimeLineFinished() {
	CleanConductors();
	LightningStrikeRadius = SphereComponent->GetScaledSphereRadius();
#ifdef UE_BUILD_DEVELOPMENT
	//UE_LOG(LogTemp, Warning, TEXT("lightningConductors numbers %d"), LightningConductors.Num());
#endif
	if (LightningConductors.Num() > 0 && CurrentTargetActor && CurrentTargetConductor) {
		if (CombinedConductors.Num() != 0) {
			const float BaseRadiusAdd = SphereComponent->GetScaledSphereRadius() * 0.5f;
			LightningStrikeRadius += BaseRadiusAdd * CombinedConductors.Num();
		}
#ifdef UE_BUILD_DEVELOPMENT
		UE_LOG(LogTemp, Warning, TEXT("Target Actor: %s"), *CurrentTargetActor->GetActorLabel());
#endif
		OnLightningStrike(CurrentTargetLocation, CurrentTargetConductor->Conductivity, CurrentTargetActor);

		// proximity, create array
		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Init(this, 1);
		TArray<AActor*> FoundActors;
		TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		UKismetSystemLibrary::SphereOverlapActors(this, CurrentTargetLocation, LightningStrikeRadius, TraceObjectTypes,
		                                          AActor::StaticClass(),
		                                          IgnoreActors, FoundActors);

		for (const auto FoundActor : FoundActors) {
			if (const auto CastedActor = Cast<AGenericActor>(FoundActor)) {
				if (CastedActor->EventInitiatorActivated) {
					Cast<IEventInitiator>(CastedActor)->Execute_OnLightningStrike(CastedActor);
				}
			}
			else if (const auto CastedPlayer = Cast<ALongLostLightningCharacter>(FoundActor)) {
				CastedPlayer->Execute_OnLightningStrike(CastedPlayer);
			}
		}
	}
	DownTimelineComponent->PlayFromStart();
	ParamChange = 0;
}

void ALightningZone::DownOnTimeLineFinished() {
	CleanConductors();
	if (LightningConductors.Num() > 0) {
		for (const auto Conductor : LightningConductors) {
			if (const auto GenericActor = Cast<AGenericActor>(Conductor)) {
				GenericActor->CalculateConductivity();
			}
			if (const auto Character = Cast<ALongLostLightningCharacter>(Conductor)) {
				Character->CalculateConductivity();
			}
		}
		SortLightningConductors();

		if (!LastTickConductor || !LastTickTargetActor) {
			// Last tick target is invalid - Set to top prio target
			CurrentTargetLocation = LightningConductors[0]->ClassReference->GetActorLocation();
			CurrentTargetActor = LightningConductors[0]->ClassReference;
			CurrentTargetConductor = LightningConductors[0];
		}
		else {
			CurrentTargetActor = LastTickTargetActor;
			CurrentTargetConductor = LastTickConductor;
			CurrentTargetLocation = LastTickTargetActor->GetActorLocation();
		}

		if (LastTickConductor != CurrentTargetConductor) {
			if (const auto GenericActor = Cast<AGenericActor>(LastTickConductor)) {
				GenericActor->Execute_OnLostTopConductor(GenericActor);
			}
			if (const auto Character = Cast<ALongLostLightningCharacter>(LastTickConductor)) {
				Character->Execute_OnLostTopConductor(Character);
			}
		}

		if (CurrentTargetActor) {
			OnLightningStarted(CurrentTargetActor, CurrentTargetLocation);
		}
	}

	UpTimelineComponent->PlayFromStart();
}

void ALightningZone::StartTimeline() {
	DownTimelineComponent->PlayFromStart();
}

// Clean out all null elements in Conductors
void ALightningZone::CleanConductors() {
	while (ConductorsDirty) {
		if (ConductorsDirty) {
			// Remove null elements
			for (auto Conductor : LightningConductors) {
				if (!Conductor) {
					LightningConductors.Remove(Conductor);
				}
			}

			// Check if array is still dirty
			bool NewDirty = false;
			for (const auto Conductor : LightningConductors) {
				if (!Conductor) {
					// Need to clean again
					NewDirty = true;
				}
			}

			// Set array clean
			if (!NewDirty) {
				ConductorsDirty = false;
			}
		}
	}
}

void ALightningZone::BeginPlay() {
	Super::BeginPlay();
	LightningConductors.Empty(); // ? Please clarify -Petter
	LightningStrikeRadius = SphereComponent->GetScaledSphereRadius();

	// Cache start pos for dynamic objects
	if (ObjToReset.Num() > 0) {
		ObjsStartPos.Empty();
		for (int i = 0; i < ObjToReset.Num(); i++) {
			if (ObjToReset[i]) {
				ObjsStartPos.Add(ObjToReset[i]->GetActorLocation());
			}
		}
	}

	for (const auto ConductorsActor : LightningConductorActors) {
		if (!ConductorsActor) {
			ConductorsDirty = true;
			continue;
		}
		if (ConductorsActor->GetClass()->ImplementsInterface(ULightningConductorInterface::StaticClass())) {
			if (const auto GenericActor = Cast<AGenericActor>(ConductorsActor)) {
				if (GenericActor->ConductorActive) {
					LightningConductors.Add(Cast<ILightningConductorInterface>(GenericActor));
				}
			}
		}
	}

	CleanConductors();

	if (UpSmoothCurve) {
		UpTimelineComponent->AddInterpFloat(UpSmoothCurve, UpInterpfunction, FName("Alpha"));
		UpTimelineComponent->SetTimelineFinishedFunc(UpInterpFinished);

		UpTimelineComponent->SetLooping(false);
	}
	if (DownSmoothCurve) {
		DownTimelineComponent->AddInterpFloat(DownSmoothCurve, DownInterpfunction, FName("Beta"));
		DownTimelineComponent->SetTimelineFinishedFunc(DownInterpFinished);

		DownTimelineComponent->SetLooping(false);
	}

	const float TimerDelay = FMath::FRandRange(1.f, 10.f);
	GetWorld()->GetTimerManager().SetTimer(ZoneRandomStartTimer, this, &ALightningZone::StartTimeline, TimerDelay,
	                                       false);
}

void ALightningZone::ResetDynamicObjects() {
	if (ObjToReset.Num() <= 0) {
		return;
	}
	for (int i = 0; i < ObjToReset.Num(); i++) {
		if (ObjToReset[i]) {
			ObjToReset[i]->SetActorLocation(ObjsStartPos[i]);
		}
	}
}

bool ALightningZone::AddConductor(TScriptInterface<ILightningConductorInterface> ConductorToAdd) {
	if (!LightningConductors.Contains(ConductorToAdd.GetInterface())) {
		if (Cast<ALongLostLightningCharacter>(ConductorToAdd.GetObject())) {
			LightningConductors.Add(ConductorToAdd.GetInterface());
		}
		else if (const auto GenericActor = Cast<AGenericActor>(ConductorToAdd.GetObject())) {
			if (GenericActor->ConductorActive) {
				LightningConductors.Add(Cast<ILightningConductorInterface>(ConductorToAdd.GetInterface()));
			}
		}
		return true;
	}
	return false;
}

bool ALightningZone::RemoveConductor(TScriptInterface<ILightningConductorInterface> ConductorToRemove) {
	if (LightningConductors.Contains(ConductorToRemove.GetInterface())) {
		if (Cast<ALongLostLightningCharacter>(ConductorToRemove.GetObject())) {
			LightningConductors.Remove(ConductorToRemove.GetInterface());
		}
		else if (const auto GenericActor = Cast<AGenericActor>(ConductorToRemove.GetObject())) {
			if (GenericActor->ConductorActive) {
				LightningConductors.Remove(Cast<ILightningConductorInterface>(ConductorToRemove.GetInterface()));
			}
		}
		return true;
	}
	return false;
}

void ALightningZone::SpawnLightningWarningEffect(UNiagaraSystem* NiagaraSystem) {
	if (LightningWarningParticleEffect) {
		return;
	}
	LightningWarningParticleEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), NiagaraSystem, FVector::ZeroVector, FRotator::ZeroRotator);
}

void ALightningZone::DestroyLightningWarningEffect() {
	if (!LightningWarningParticleEffect) {
		UE_LOG(LogTemp, Warning, TEXT("Niagara System not valid!"))
	}
	LightningWarningParticleEffect->DestroyComponent();
	LightningWarningParticleEffect = nullptr;
}

void ALightningZone::SortLightningConductors() {
	// Called in timeline
	LightningConductors.Sort();
}

/**
 * Deprecated because Unreal magic. GetOverLappingActors is unreliable when switching levels
 */
void ALightningZone::AddAllConductors() {
	TArray<AActor*> foundActors;
	LightningZoneTrigger->SetActive(false);
	LightningZoneTrigger->SetActive(true);
	LightningZoneTrigger->GetOverlappingActors(foundActors);

	for (AActor* FoundActor : foundActors) {
		if (FoundActor->GetClass()->ImplementsInterface(ULightningConductorInterface::StaticClass())) {
			auto ConductorInterface = Cast<ILightningConductorInterface>(FoundActor);
			if (!LightningConductors.Contains(ConductorInterface)) {
				if (Cast<ALongLostLightningCharacter>(ConductorInterface)) {
					UE_LOG(LogTemp, Warning, TEXT("Added Player"))
					LightningConductors.Add(ConductorInterface);
				}
				else if (const auto CastedActor = CastChecked<AGenericActor>(FoundActor)) {
					if (CastedActor && CastedActor->ConductorActive) {
#ifdef UE_BUILD_DEVELOPMENT
						UE_LOG(LogTemp, Warning, TEXT("Casted actor:  %s %ls"), ( CastedActor->ConductorActive ? TEXT("true") : TEXT("false") ),
						       *CastedActor->GetActorLabel());
#endif
						LightningConductors.Add(Cast<ILightningConductorInterface>(CastedActor));
					}
				}
			}
		}
	}

	SortLightningConductors();
}

// Called every frame
void ALightningZone::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
