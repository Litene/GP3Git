// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FConductivityModStruct.generated.h"

/**
 * 
 */
USTRUCT()
struct FConductivityModStruct {
	GENERATED_BODY()

	FConductivityModStruct();
	FConductivityModStruct(EModifier InputModifier);

	EModifier Modifier;
	bool Activated = false;
};


UENUM(BlueprintType)
enum class EModifier : uint8 {
	Water = 20 UMETA(DisplayName="Water"),
	Mud = 5 UMETA(DisplayName="Mud"),
	Fire = 15 UMETA(DisplayName="Fire"),
	None = 0 UMETA(DisplayName="None")
};
