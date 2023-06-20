// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericActor.h"
#include "PuzzleDoor.generated.h"

/**
 * 
 */
UCLASS()
class LONGLOSTLIGHTNING_API APuzzleDoor : public AGenericActor
{
	GENERATED_BODY()
public:
		UPROPERTY(BlueprintReadWrite)
    	TArray<AGenericActor*> Locks;
    	
    	void TryUnlock();
    
    	UFUNCTION(BlueprintImplementableEvent)
    	void UnlockDoor();

		UFUNCTION(BlueprintCallable)
    	void AddToLock(AGenericActor* ActorLock);
		UFUNCTION(BlueprintCallable)
    	void RemoveFromLock(AGenericActor* ActorLock);
};
