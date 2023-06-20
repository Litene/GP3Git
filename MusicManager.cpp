// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicManager.h"

#include "Components/AudioComponent.h"


// Sets default values
AMusicManager::AMusicManager() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MetaAudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	SetRootComponent(MetaAudioComponent);
}

void AMusicManager::SetArrangement(bool LightPercussion, bool Uke1, bool BassOrgan1, bool BassOrgan2, bool Heavypercussion1, bool Uke2,
                                   bool ElectricGuitar, bool HeavyPercussion2, bool Uke3, bool BassOrgan3, bool BassOrgan4, bool MediumPercussion,
                                   bool Synth, bool Woodwind, bool Track15, bool Track16) {
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track1, LightPercussion);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track2, Uke1);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track3, BassOrgan1);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track4, BassOrgan2);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track5, Heavypercussion1);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track6, Uke2);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track7, ElectricGuitar);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track8, HeavyPercussion2);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track9, Uke3);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track10, BassOrgan3);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track11, BassOrgan4);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track12, MediumPercussion);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track13, Synth);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track14, Woodwind);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track15, Track15);
	MetaAudioComponent->SetFloatParameter(MusicArrangementNames.Track16, Track16);
}

// Called when the game starts or when spawned
void AMusicManager::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AMusicManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
