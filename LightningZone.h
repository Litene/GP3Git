// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ILightningConductorInterface.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/Interface.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "LightningZone.generated.h"


UCLASS()
class LONGLOSTLIGHTNING_API ALightningZone : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightningZone();

	UPROPERTY(Instanced, EditInstanceOnly)
	TObjectPtr<UBoxComponent> LightningZoneTrigger;
	
	UPROPERTY(EditAnywhere, Category="Ref")
	UCurveFloat* DownSmoothCurve;
	UPROPERTY(EditAnywhere, Category="Ref")
	UCurveFloat* UpSmoothCurve;
	
	UPROPERTY(EditAnywhere, Category="Ref")
	UTimelineComponent* DownTimelineComponent;
	UPROPERTY(EditAnywhere, Category="Ref")
	UTimelineComponent* UpTimelineComponent;
	bool ConductorsDirty;

	UFUNCTION()
	void UpTimelineFloatReturn(float Value);
	UFUNCTION()
	void DownTimelineFloatReturn(float Value);

	UFUNCTION()
	void UpOnTimeLineFinished();
	UFUNCTION()
	void DownOnTimeLineFinished();
	
	void StartTimeline();
	void CleanConductors();
	void SetCurrentTargetNotRandomized();
	void OnLostTarget();

	FOnTimelineFloat UpInterpfunction{};
	FOnTimelineFloat DownInterpfunction{};

	FOnTimelineEvent UpInterpFinished{};
	FOnTimelineEvent DownInterpFinished{};

	UPROPERTY()
	FTimerHandle ZoneRandomStartTimer;

	TArray<AActor*> CombinedConductors;

	FVector CurrentTargetLocation;
	AActor* CurrentTargetActor;
	AActor* LastTickTargetActor;
	ILightningConductorInterface* LastTickConductor;

	ILightningConductorInterface* CurrentTargetConductor;
	float UpdateDateCooldown = 0.3f;
	float UpdateTimer;
	float ParamChange;
	float LightningStrikeRadius;

	UFUNCTION(BlueprintImplementableEvent)
	void OnConductorsCalculated(const AActor* CurrentTarget);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLightningStarted(const AActor* TargetActor, FVector TargetLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLightningTick(const AActor* currentTarget, float ReturnValue);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNoTarget();

	TArray<ILightningConductorInterface*> LightningConductors;

	UPROPERTY(BlueprintReadOnly)
	UNiagaraComponent* LightningWarningParticleEffect;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> LightningConductorActors;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> ObjToReset;
	TArray<FVector> ObjsStartPos;
	
	UFUNCTION(BlueprintCallable)
	void ResetDynamicObjects();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLightningStrike(FVector TargetLocation, float TargetConductivity, AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	bool AddConductor(TScriptInterface<ILightningConductorInterface> ConductorToAdd);
	UFUNCTION(BlueprintCallable)
	bool RemoveConductor(TScriptInterface<ILightningConductorInterface> ConductorToRemove);

	UFUNCTION(BlueprintCallable)
	void SpawnLightningWarningEffect(UNiagaraSystem* NiagaraSystem);
	UFUNCTION(BlueprintCallable)
	void DestroyLightningWarningEffect();

	void SortLightningConductors();
	void AddAllConductors();

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
