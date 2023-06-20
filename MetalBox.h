// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILightningConductorInterface.h"
#include "GameFramework/Actor.h"
#include "LongLostLightning/EventInitiator.h"
#include "MetalBox.generated.h"

UCLASS()
class LONGLOSTLIGHTNING_API AMetalBox : public AActor, public ILightningConductorInterface, public IEventInitiator {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMetalBox();

	UFUNCTION(BlueprintCallable)
	virtual void SetModifier(EModifier NewModifier) override;
	UFUNCTION(BlueprintGetter)
	virtual EModifier GetModifier() override;
	UFUNCTION(BlueprintGetter)
	virtual int32 GetBaseConductivity() override;
	UFUNCTION(BlueprintCallable)
	virtual float CalculateConductivity() override;
	UFUNCTION(BlueprintCallable)
	virtual void SetBaseConductivity(int32 ConductivityValue) override;
	UFUNCTION(BlueprintCallable)
	virtual void SetFloorIncrementValue(int32 IncrementValue) override;
	UFUNCTION(BlueprintCallable)
	virtual void AddNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) override;
	UFUNCTION(BlueprintCallable)
	virtual void RemoveNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) override;

	//virtual void OnLightningStrike_Implementation() override;

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnLightningStrike();
	
	UPROPERTY(EditDefaultsOnly, Category="Ref")
	TObjectPtr<UStaticMeshComponent> BoxMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
