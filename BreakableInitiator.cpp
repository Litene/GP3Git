// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableInitiator.h"


// Sets default values
ABreakableInitiator::ABreakableInitiator() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetClassReference(this);
}

void ABreakableInitiator::SetModifier(EModifier NewModifier) {
	ILightningConductorInterface::SetModifier(NewModifier);
}

EModifier ABreakableInitiator::GetModifier() {
	return ILightningConductorInterface::GetModifier();
}

int32 ABreakableInitiator::GetBaseConductivity() {
	return ILightningConductorInterface::GetBaseConductivity();
}

float ABreakableInitiator::CalculateConductivity() {
	ILightningConductorInterface::CalculateHeightValue(GetActorLocation().Z);
	return ILightningConductorInterface::CalculateConductivity();
}

void ABreakableInitiator::SetBaseConductivity(int32 ConductivityValue) {
	ILightningConductorInterface::SetBaseConductivity(ConductivityValue);
}

void ABreakableInitiator::SetFloorIncrementValue(int32 IncrementValue) {
	ILightningConductorInterface::SetFloorIncrementValue(IncrementValue);
}

void ABreakableInitiator::AddNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface::AddNearbyConductor(OtherConductor);
}

void ABreakableInitiator::RemoveNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface::RemoveNearbyConductor(OtherConductor);
}

void ABreakableInitiator::ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome) {
	IEventInitiator::ChanceModifier(PercentChance, Outcome);
}

void ABreakableInitiator::RepeatingModifier(int TimesToRepeat, float TimeInBetween) {
	IEventInitiator::RepeatingModifier(TimesToRepeat, TimeInBetween);
}

// Called when the game starts or when spawned
void ABreakableInitiator::BeginPlay() {
	Super::BeginPlay();

	if (!ObjectToAffectRef) {
		return;
	}

	ObjectToAffect = TScriptInterface<IEventListener>(ObjectToAffectRef);
	if (!ObjectToAffect) {
		// UE_LOG(LogTemp, Warning, TEXT("EventListener Unensured, Uinsured and Unassured. Please check reference or call programmers! %s"), *GetActorLabel())
	}
}

// Called every frame
void ABreakableInitiator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
