// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventListener.h"
#include "GameFramework/Actor.h"
#include "MovableWall.generated.h"

UCLASS()
class LONGLOSTLIGHTNING_API AMovableWall : public AActor, public IEventListener {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovableWall();
	
	virtual void OnEventAction_Implementation(AGenericActor* CallingActor) override;

	UPROPERTY(EditDefaultsOnly, Category="Ref")
	TObjectPtr<UStaticMeshComponent> BoxMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
