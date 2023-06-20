// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericActor.h"

#include "EngineUtils.h"
#include "LongLostLightning/MusicManager.h"

// Sets default values
AGenericActor::AGenericActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetClassReference(this);
}

void AGenericActor::SetModifier(EModifier NewModifier) {
	ILightningConductorInterface::SetModifier(NewModifier);
}

EModifier AGenericActor::GetModifier() {
	return ILightningConductorInterface::GetModifier();
}

int32 AGenericActor::GetBaseConductivity() {
	return ILightningConductorInterface::GetBaseConductivity();
}

float AGenericActor::CalculateConductivity() {
	ILightningConductorInterface::CalculateHeightValue(GetActorLocation().Z);
	return ILightningConductorInterface::CalculateConductivity();
}

void AGenericActor::SetBaseConductivity(int32 ConductivityValue) {
	ILightningConductorInterface::SetBaseConductivity(ConductivityValue);
}

void AGenericActor::SetFloorIncrementValue(int32 IncrementValue) {
	ILightningConductorInterface::SetFloorIncrementValue(IncrementValue);
}

void AGenericActor::SetFloorHeight(int32 FloorHeightValue) {
	ILightningConductorInterface::SetFloorHeight(FloorHeightValue);
}

void AGenericActor::AddNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface::AddNearbyConductor(OtherConductor);
}

void AGenericActor::RemoveNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface::RemoveNearbyConductor(OtherConductor);
}

void AGenericActor::SetSharedValue(int32 SharedValue) {
	ILightningConductorInterface::SetSharedValue(SharedValue);
}

void AGenericActor::ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome) {
	IEventInitiator::ChanceModifier(PercentChance, Outcome);
}

void AGenericActor::RepeatingModifier(int TimesToRepeat, float TimeInBetween) {
	IEventInitiator::RepeatingModifier(TimesToRepeat, TimeInBetween);
}

void AGenericActor::OnEventAction_Implementation(AGenericActor* CallingActor) {
	IEventListener::OnEventAction_Implementation(CallingActor);
}

void AGenericActor::OnEventActionDetach_Implementation(AGenericActor* CallingActor) {
	IEventListener::OnEventAction_Implementation(CallingActor);
}

void AGenericActor::OnNoStormMusic_Implementation() {
	IEventListener::OnNoStormMusic_Implementation();
}

void AGenericActor::OnLowStormMusic_Implementation() {
	IEventListener::OnLowStormMusic_Implementation();
}

void AGenericActor::OnMediumStormMusic_Implementation() {
	IEventListener::OnMediumStormMusic_Implementation();
}

void AGenericActor::OnHighStormMusic_Implementation() {
	IEventListener::OnHighStormMusic_Implementation();
}

void AGenericActor::SetProperties(bool Conductor, bool Initiator, bool Listener) {
	ConductorActive = Conductor;
	EventListenerActivated = Listener;
	EventInitiatorActivated = Initiator;
}

// Called when the game starts or when spawned
void AGenericActor::BeginPlay() {
	Super::BeginPlay();

	if (ObjectsToAffectRef.Num() > 0) {
		for (auto ObjectToAffectRef : ObjectsToAffectRef) {
			ObjectsToAffect.Add(TScriptInterface<IEventListener>(ObjectToAffectRef));
		}
	}

	for (TActorIterator<AMusicManager> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator) {
		MusicListenerActor = *ActorIterator;
	}

	MusicEventListener = TScriptInterface<IEventListener>(MusicListenerActor);
#ifdef UE_EDITOR
	if (!MusicEventListener) {
		UE_LOG(LogTemp, Warning, TEXT("Music Event Listener reference is unassured, unensured and unissured. Call programmers! %s"), *GetName())
	}
#endif
}

// Called every frame
void AGenericActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
