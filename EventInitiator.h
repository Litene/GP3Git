// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventListener.h"
#include "UObject/Interface.h"
#include "EventInitiator.generated.h"

UENUM(BlueprintType)
enum EOutcomePins {
	Success,
	Failure
};

// This class does not need to be modified.
UINTERFACE()
class UEventInitiator : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class LONGLOSTLIGHTNING_API IEventInitiator {
	GENERATED_BODY()


	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	IEventListener* EventListener;

	// MODIFIERS
	virtual void ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome);
	virtual void RepeatingModifier(int TimesToRepeat, float TimeInBetween);

	// Conditions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLightningStrike();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteract();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnTopConductor();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnLostTopConductor();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEnter();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnExit();

	//Execute
	virtual void SendToObject();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TimerCallBackEvent();
};
