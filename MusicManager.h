// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericActor.h"
#include "MusicManager.generated.h"

USTRUCT(BlueprintType)
struct FMusicArrangementNames {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName Track1 = "PlayTrack01";
	UPROPERTY(BlueprintReadOnly)
	FName Track2 = "PlayTrack02";
	UPROPERTY(BlueprintReadOnly)
	FName Track3 = "PlayTrack03";
	UPROPERTY(BlueprintReadOnly)
	FName Track4 = "PlayTrack04";
	UPROPERTY(BlueprintReadOnly)
	FName Track5 = "PlayTrack05";
	UPROPERTY(BlueprintReadOnly)
	FName Track6 = "PlayTrack06";
	UPROPERTY(BlueprintReadOnly)
	FName Track7 = "PlayTrack07";
	UPROPERTY(BlueprintReadOnly)
	FName Track8 = "PlayTrack08";
	UPROPERTY(BlueprintReadOnly)
	FName Track9 = "PlayTrack09";
	UPROPERTY(BlueprintReadOnly)
	FName Track10 = "PlayTrack10";
	UPROPERTY(BlueprintReadOnly)
	FName Track11 = "PlayTrack11";
	UPROPERTY(BlueprintReadOnly)
	FName Track12 = "PlayTrack12";
	UPROPERTY(BlueprintReadOnly)
	FName Track13 = "PlayTrack13";
	UPROPERTY(BlueprintReadOnly)
	FName Track14 = "PlayTrack14";
	UPROPERTY(BlueprintReadOnly)
	FName Track15 = "PlayTrack15";
	UPROPERTY(BlueprintReadOnly)
	FName Track16 = "PlayTrack16";
};

UCLASS()
class LONGLOSTLIGHTNING_API AMusicManager : public AGenericActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMusicManager();

	FMusicArrangementNames MusicArrangementNames;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UAudioComponent> MetaAudioComponent;

	UFUNCTION(BlueprintCallable)
	void SetArrangement(bool LightPercussion, bool Uke1, bool BassOrgan1, bool BassOrgan2, bool Heavypercussion1, bool Uke2, bool ElectricGuitar,
	                    bool HeavyPercussion2, bool Uke3, bool BassOrgan3, bool BassOrgan4, bool MediumPercussion, bool Synth, bool Woodwind, bool Track15,
	                    bool Track16);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
