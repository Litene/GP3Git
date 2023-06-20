// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableWall.h"


// Sets default values
AMovableWall::AMovableWall() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoxMesh");
	SetRootComponent(BoxMesh);
}

void AMovableWall::OnEventAction_Implementation(AGenericActor* CallingActor) { //rename

	UE_LOG(LogTemp, Warning, TEXT("Event action"))
	IEventListener::OnEventAction_Implementation(CallingActor);
}

// Called when the game starts or when spawned
void AMovableWall::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AMovableWall::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

