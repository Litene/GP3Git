// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningRod.h"

#include "Components/ArrowComponent.h"


// Sets default values
ALightningRod::ALightningRod() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALightningRod::BeginPlay() {
	Super::BeginPlay();
}

void ALightningRod::ApplyGravity() {
	FVector MyLoc = this->GetActorLocation();
	if(!IInteractableInterface::GroundCheck(this,MyLoc,GetWorld()) && !AttachedToPlayer) {
		this->SetActorLocation(FVector(MyLoc.X,MyLoc.Y,MyLoc.Z-Gravity));
	}
}

// Called every frame
void ALightningRod::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	ApplyGravity();
}

void ALightningRod::AddArrayToArray(TArray<AActor*> Array) {
	//Get array of all actors with interface from blueprint and add to our array
	for (auto Actor : Array) {
		LightningRods.Add(Actor);
	}
	LightningRods.Remove(this); //Remove self from available rods(in array)
}

void ALightningRod::SignalNextRod() {

	if (ObjectsToAffect.Num() > 0) {
		OnSignalObjectsToEffect();
	}

	OnAttemptToSend();
	
	//Get nearest rod, activate it and ask to find new rod if any.
	AActor* Rod = FindNearestLightningRod();
	if (Rod != nullptr) {
		ALightningRod* RodCCP = Cast<ALightningRod>(Rod);
		RodCCP->ActiveByLightning = true;
		RodCCP->DelaySendingSignal();
		
		TargetRodArrowComponent = Cast<USceneComponent>(Rod->GetComponentByClass(UArrowComponent::StaticClass()));
		
		const FVector MyLoc = GetActorLocation();
		const FVector RodLoc = Rod->GetActorLocation();
		const FVector Offset = FVector(0,0,200);

		OnSignalSent();
	}
}

void ALightningRod::DelaySendingSignal() {
	//Set timer before sending signal in seconds
	GetWorld()->GetTimerManager().SetTimer(SignalTimerHandle, this, &ALightningRod::SignalNextRod, SignalDelay, false);
}


AActor* ALightningRod::FindNearestLightningRod() {

	TArray<AActor*> MyNearestRods;

	//Add all rods that are currently inactive to our array
	for (auto Rod : LightningRods) {
		if (!Cast<ALightningRod>(Rod)->ActiveByLightning) {
			if (Rod->GetDistanceTo(this) <= SignalRange)
				MyNearestRods.Add(Rod);
		}
	}
	//Sort our array by distance
	if (MyNearestRods.Num() == 0) return nullptr;
	if (MyNearestRods.Num() > 1) {
		MyNearestRods.Sort([this](const AActor& a, const AActor& b) {
			return a.GetDistanceTo(this) < b.GetDistanceTo(this);
		});
	}
	return MyNearestRods[0];
}
