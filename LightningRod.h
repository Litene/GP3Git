// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GenericActor.h>

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "LightningRodInterface.h"
#include "GameFramework/Actor.h"
#include "LightningRod.generated.h"

UCLASS()
class LONGLOSTLIGHTNING_API ALightningRod : public AGenericActor, public ILightningRodInterface, public IInteractableInterface {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightningRod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Gravity = 8;
	
	void ApplyGravity();

	TArray<AActor*> LightningRods;

	FTimerHandle SignalTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		float SignalDelay = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		float SignalRange = 1000;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* FindNearestLightningRod();

	UFUNCTION(BlueprintCallable)
		void AddArrayToArray(TArray<AActor*> Array);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
		bool ActiveByLightning;

	UPROPERTY(BlueprintReadWrite)
		FVector TargetRodLocation;

	UPROPERTY(BlueprintReadWrite)
		USceneComponent* TargetRodArrowComponent;

	UFUNCTION(BlueprintCallable)
		void SignalNextRod();

	UFUNCTION(BlueprintCallable)
		void DelaySendingSignal();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSignalObjectsToEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSignalSent();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttemptToSend();
};
