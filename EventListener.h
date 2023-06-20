// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EventListener.generated.h"

class AGenericActor;
// This class does not need to be modified.
UINTERFACE()
class UEventListener : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class LONGLOSTLIGHTNING_API IEventListener {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEventAction(AGenericActor* CallingActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEventActionDetach(AGenericActor* CallingActor);
	
	// Music Events
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnNoStormMusic();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnLowStormMusic();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMediumStormMusic();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHighStormMusic();
};
