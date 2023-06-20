// Fill out your copyright notice in the Description page of Project Settings.


#include "LongLostLightning/Public/ILightningConductorInterface.h"
#include "LongLostLightning/Public/FConductivityModStruct.h"

ILightningConductorInterface::ILightningConductorInterface() {
	CurrentModifier = EModifier::None;
	FloorIncrementValue = 5;
	FloorHeight = 100;
}

void ILightningConductorInterface::SetClassReference(TObjectPtr<AActor> SelfRef) {
	ClassReference = SelfRef;
}

void ILightningConductorInterface::SetBaseConductivity(int32 ConductivityValue) {
	BaseConductivity = ConductivityValue;
}

void ILightningConductorInterface::SetSharedValue(int32 SharedValue) {
	ConductivitySharedValue = SharedValue;
}

void ILightningConductorInterface::SetModifier(EModifier NewModifier) {
	CurrentModifier = NewModifier;
}

void ILightningConductorInterface::SetFloorHeight(int32 FloorHeightValue) {
	FloorHeight = FloorHeightValue;
}

void ILightningConductorInterface::SetFloorIncrementValue(int32 IncrementValue) {
	FloorIncrementValue = IncrementValue;
}

bool ILightningConductorInterface::IsPlayerConductor() {
	return IsPlayer;
}

EModifier ILightningConductorInterface::GetModifier() {
	return CurrentModifier;
}

int32 ILightningConductorInterface::GetBaseConductivity() {
	return BaseConductivity;
}

float ILightningConductorInterface::CalculateConductivity() {
	// TODO: We calculate height value here and on classes who implements interface ?? Please clarify -Petter too bad!!
	CalculateHeightValue(ClassReference->GetActorLocation().Z);
	if (static_cast<uint8>(CurrentModifier) == 0) {
		Conductivity = BaseConductivity + HeightValue + NearbyConductivity;
		return Conductivity;
	}
	Conductivity = BaseConductivity * static_cast<float>(static_cast<uint8>(CurrentModifier)) / 10 + HeightValue + NearbyConductivity;
	return Conductivity;
}

void ILightningConductorInterface::CalculateHeightValue(float ActorZ) {
	if (FloorHeight == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to divide floor height by zero, please check math"))
		return;
	}


	const int32 MidBounds = FMath::FloorToInt(ActorZ / FloorHeight);
	const int32 UpperBounds = FMath::FloorToInt((ActorZ + 5.0f) / FloorHeight);
	if (MidBounds == UpperBounds) { HeightValue = MidBounds * FloorIncrementValue; }
	else { HeightValue = UpperBounds * FloorIncrementValue; }
}

void ILightningConductorInterface::AddNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface* OtherInterface = OtherConductor.GetInterface();
	if (!NearbyConductors.Contains(OtherInterface)) {
		NearbyConductors.Add(OtherInterface);
		NearbyConductivity += OtherInterface->ConductivitySharedValue;
	}
}

void ILightningConductorInterface::RemoveNearbyConductor(
	TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface* OtherInterface = OtherConductor.GetInterface();
	if (NearbyConductors.Contains(OtherInterface)) {
		NearbyConductors.Remove(OtherInterface);
		NearbyConductivity -= OtherInterface->ConductivitySharedValue;
	}
}
