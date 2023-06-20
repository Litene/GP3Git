// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FConductivityModStruct.h"
#include "ILightningConductorInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class ULightningConductorInterface : public UInterface {
	GENERATED_BODY()
};

class LONGLOSTLIGHTNING_API ILightningConductorInterface {
	GENERATED_BODY()

public:
	ILightningConductorInterface();

	TArray<ILightningConductorInterface*> NearbyConductors;
	int32 BaseConductivity;
	float Conductivity;
	int32 ConductivitySharedValue;
	int32 NearbyConductivity;
	int32 HeightValue;
	int32 FloorIncrementValue;
	int32 FloorHeight;
	bool IsPlayer;

	bool operator <(const ILightningConductorInterface& OtherConductor) const { 
		if (Conductivity > OtherConductor.Conductivity) {
			return true;
		}
		return false;
	}

	TObjectPtr<AActor> ClassReference;
	
	void SetClassReference(TObjectPtr<AActor> SelfRef);
	
	virtual void SetBaseConductivity(int32 ConductivityValue);
	virtual void SetSharedValue(int32 SharedValue);
	virtual void SetModifier(EModifier NewModifier);
	virtual void SetFloorHeight(int32 FloorHeightValue);
	virtual void SetFloorIncrementValue(int32 IncrementValue);
	virtual bool IsPlayerConductor();
	virtual EModifier GetModifier();
	virtual int32 GetBaseConductivity();
	virtual float CalculateConductivity();
	virtual void CalculateHeightValue(float ActorZ);
	virtual void AddNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor);
	virtual void RemoveNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor);

protected:
	EModifier CurrentModifier;
};
