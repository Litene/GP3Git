// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GenericActor.h>

#include "CoreMinimal.h"
#include "LongLostLightning/InteractableInterface.h"
#include "Interactable.generated.h"

UCLASS()
class LONGLOSTLIGHTNING_API AInteractable : public AGenericActor, public IInteractableInterface
{

	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();


private:


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplyGravity();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float Gravity = 8;
	
};

