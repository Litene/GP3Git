// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleDoor.h"

void APuzzleDoor::TryUnlock() {
	if (Locks.Num() == 2) UnlockDoor();
	
}

void APuzzleDoor::AddToLock(AGenericActor* ActorLock) {
	if (!Locks.Contains(ActorLock)) {
		Locks.Add(ActorLock);
	}
	TryUnlock();
}

void APuzzleDoor::RemoveFromLock(AGenericActor* ActorLock) {
	if (Locks.Contains(ActorLock)) {
		Locks.Remove(ActorLock);
	}
}