// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILightningConductorInterface.h"
#include "GameFramework/Actor.h"
#include "LongLostLightning/EventInitiator.h"
#include "GenericActor.generated.h"


UCLASS()
class LONGLOSTLIGHTNING_API AGenericActor : public AActor, public IEventInitiator, public IEventListener,
                                            public ILightningConductorInterface {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGenericActor();

	// CONDUCTOR
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
	virtual void SetFloorHeight(int32 FloorHeightValue) override;
	UFUNCTION(BlueprintCallable)
	virtual void AddNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) override;
	UFUNCTION(BlueprintCallable)
	virtual void RemoveNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) override;
	UFUNCTION(BlueprintCallable)
	virtual void SetSharedValue(int32 SharedValue) override;

	// EVENT INITIATOR
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
	virtual void ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome) override;
	UFUNCTION(BlueprintCallable)
	virtual void RepeatingModifier(int TimesToRepeat, float TimeInBetween) override;

	// EVENT LISTENER
	virtual void OnEventAction_Implementation(AGenericActor* CallingActor) override;

	virtual void OnEventActionDetach_Implementation(AGenericActor* CallingActor) override;

	// MUSIC EVENTS
	virtual void OnNoStormMusic_Implementation() override;
	virtual void OnLowStormMusic_Implementation() override;
	virtual void OnMediumStormMusic_Implementation() override;
	virtual void OnHighStormMusic_Implementation() override;

	// CONSTRUCTOR
	UFUNCTION(BlueprintCallable)
	void SetProperties(bool Conductor, bool Initiator, bool Listener);
	UPROPERTY(VisibleAnywhere)
	bool ConductorActive;
	UPROPERTY(VisibleAnywhere)
	bool EventInitiatorActivated;
	UPROPERTY(VisibleAnywhere)
	bool EventListenerActivated;

	UPROPERTY(EditInstanceOnly, Category="Event Setup")
	TArray<AActor*> ObjectsToAffectRef;
	UPROPERTY(BlueprintReadOnly)
	TArray<TScriptInterface<IEventListener>> ObjectsToAffect;

	UPROPERTY()
	AActor* MusicListenerActor;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IEventListener> MusicEventListener;

	UPROPERTY(BlueprintReadOnly, Category="Music Setup")
	AActor* MusicManager;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
