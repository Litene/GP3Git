// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventInitiator.h"
#include "ILightningConductorInterface.h"
#include "GameFramework/Actor.h"
#include "BreakableInitiator.generated.h"

UCLASS()
class LONGLOSTLIGHTNING_API ABreakableInitiator : public AActor, public IEventInitiator, public ILightningConductorInterface {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableInitiator();

	// Lightning Conductor
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
	
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
	virtual void ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome) override;
	UFUNCTION(BlueprintCallable)
	virtual void RepeatingModifier(int TimesToRepeat, float TimeInBetween) override;

	UPROPERTY(EditInstanceOnly, Category="Event Setup")
	AActor* ObjectToAffectRef;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SendToObject() override;

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IEventListener> ObjectToAffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
