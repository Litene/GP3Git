// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableInterface.h"


// Add default functionality here for any IInteractableInterface functions that are not pure virtual.
void IInteractableInterface::OnInteract() {
}

bool IInteractableInterface::GroundCheck(AActor* Actor,FVector ActorLocation, UWorld* World) {

	FVector TraceStart = ActorLocation + FVector::UpVector*50;
	FVector TraceEnd = ActorLocation + FVector::DownVector;
	FHitResult GroundedHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Actor);

	World->LineTraceSingleByChannel(GroundedHit, TraceStart, TraceEnd, ECC_WorldDynamic, QueryParams);
    
	return GroundedHit.bBlockingHit && IsValid(GroundedHit.GetActor());

}