// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EventInitiator.h"
#include "ILightningConductorInterface.h"
#include "InputActionValue.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "LongLostLightningCharacter.generated.h"



UCLASS(config=Game)
class ALongLostLightningCharacter : public ACharacter, public ILightningConductorInterface, public IEventInitiator {
	GENERATED_BODY()
	

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

public:
	ALongLostLightningCharacter();

	/** Conductor */
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

	/** Event Initiator */
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
	virtual void ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome) override;
	UFUNCTION(BlueprintCallable)
	virtual void RepeatingModifier(int TimesToRepeat, float TimeInBetween) override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SendToObject() override;

	//UPROPERTY(BlueprintReadWrite)
	bool Invincible;
	//ECharacterState CharState;

	float InvincibleTimer;
	float InvincibleCooldown;

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
	virtual void GetInvincibility(TEnumAsByte<EOutcomePins>& Outcome);

	UFUNCTION(BlueprintCallable)
	void TriggerInvincibility(float TimeInvincible = 0.3f, bool LowerConductivity = false);


	UFUNCTION(BlueprintCallable)
	void SetCooldown(float TimeInvincible);

	UPROPERTY
	(EditInstanceOnly, Category="Event Setup")
	AActor* ObjectToAffectRef;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IEventListener> ObjectToAffect;
	UPROPERTY(BlueprintReadWrite)
	bool IsDead;

	UFUNCTION(BlueprintCallable)
	void SetProperties(bool Conductor, bool Initiator, bool ConductorIsPlayer);

	UPROPERTY(VisibleAnywhere)
	bool ConductorActive;
	UPROPERTY(VisibleAnywhere)
	bool EventInitiatorActivated;
	UPROPERTY(VisibleAnywhere)
	bool PlayerConductorActive;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	UBoxComponent* InteractableDetectionBox;

	UFUNCTION(BlueprintCallable)
	AActor* Grab(AActor* OverlappedInteractable);
	UFUNCTION(BlueprintCallable)
	void StopGrab();

	UFUNCTION(BlueprintCallable)
	void AddInteractableToArray(AActor* OverlappedObject);

	UFUNCTION(BlueprintCallable)
	void RemoveInteractableFromArray(AActor* OverlappedObject);

	UFUNCTION(BlueprintCallable)
	AActor* GetClosestInteractable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	UPROPERTY(BlueprintReadWrite)
	bool Dodging;
	UPROPERTY(BlueprintReadWrite)
	bool Grabbing;

protected:
	void SurfaceGrabbingActor();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void DodgeController();
	void StopDodge();
	void Dodge();
	float CurrentDodgeCooldown;
	FTimerHandle DodgeRecoveryHandle;
	void DodgeRecoveryFrames();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
	float RecoveryTime = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge)
	float DodgeStrength = 1600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodgee)
	float DodgeCooldown = 1;

	TArray<AActor*> InteractableArray;

	AActor* ActorGrabbing;

	FRotator MoveRotation;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings)
	float SensitivityXAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings)
	float SensitivityYAxis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings)
	float DirectionRotationRate;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDodge();
};


// UENUM(BlueprintType)
// enum class ECharacterState : uint8 {
// 	Normal = 0 UMETA(DisplayName="Normal"),
// 	Climbing = 1 UMETA(DisplayName="Climbing"),
// 	Invincible = 2 UMETA(DisplayName="Invincible"),
// };
