// Copyright Epic Games, Inc. All Rights Reserved.

#include "LongLostLightningCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interactable.h"
#include "InteractableInterface.h"
#include "Kismet/KismetMathLibrary.h"


//////////////////////////////////////////////////////////////////////////
// ALongLostLightningCharacter

ALongLostLightningCharacter::ALongLostLightningCharacter() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InteractableDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableDetectionBox"));
	InteractableDetectionBox->SetupAttachment(RootComponent);
	//CharState = ECharacterState::Normal;

	SetClassReference(this);
}


void ALongLostLightningCharacter::SetModifier(EModifier NewModifier) {
	ILightningConductorInterface::SetModifier(NewModifier);
}

EModifier ALongLostLightningCharacter::GetModifier() {
	return ILightningConductorInterface::GetModifier();
}

int32 ALongLostLightningCharacter::GetBaseConductivity() {
	return ILightningConductorInterface::GetBaseConductivity();
}

float ALongLostLightningCharacter::CalculateConductivity() {
	ILightningConductorInterface::CalculateHeightValue(GetActorLocation().Z);
	return ILightningConductorInterface::CalculateConductivity();
}

void ALongLostLightningCharacter::SetBaseConductivity(int32 ConductivityValue) {
	ILightningConductorInterface::SetBaseConductivity(ConductivityValue);
}

void ALongLostLightningCharacter::SetFloorIncrementValue(int32 IncrementValue) {
	ILightningConductorInterface::SetFloorIncrementValue(IncrementValue);
}

void ALongLostLightningCharacter::SetFloorHeight(int32 FloorHeightValue) {
	ILightningConductorInterface::SetFloorHeight(FloorHeightValue);
}

void ALongLostLightningCharacter::AddNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface::AddNearbyConductor(OtherConductor);
}

void ALongLostLightningCharacter::RemoveNearbyConductor(TScriptInterface<ILightningConductorInterface> OtherConductor) {
	ILightningConductorInterface::RemoveNearbyConductor(OtherConductor);
}

void ALongLostLightningCharacter::SetSharedValue(int32 SharedValue) {
	ILightningConductorInterface::SetSharedValue(SharedValue);
}

void ALongLostLightningCharacter::ChanceModifier(int PercentChance, TEnumAsByte<EOutcomePins>& Outcome) {
	IEventInitiator::ChanceModifier(PercentChance, Outcome);
}

void ALongLostLightningCharacter::RepeatingModifier(int TimesToRepeat, float TimeInBetween) {
	IEventInitiator::RepeatingModifier(TimesToRepeat, TimeInBetween);
}


void ALongLostLightningCharacter::GetInvincibility(TEnumAsByte<EOutcomePins>& Outcome) {
	Outcome = Invincible ? Success : Failure;
}

void ALongLostLightningCharacter::TriggerInvincibility(float TimeInvincible, bool LowerConductivity) {
	InvincibleCooldown = TimeInvincible;
	InvincibleTimer = 0;
	Invincible = true;

	if (LowerConductivity) {
		Conductivity = 0;
	}
}

void ALongLostLightningCharacter::SetCooldown(float TimeInvincible) {
	InvincibleCooldown = TimeInvincible;
}

void ALongLostLightningCharacter::SetProperties(bool Conductor, bool Initiator, bool ConductorIsPlayer) {
	ConductorActive = Conductor;
	EventInitiatorActivated = Initiator;
	PlayerConductorActive = ConductorIsPlayer;
}

void ALongLostLightningCharacter::BeginPlay() {
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Event Initiator
	if (!ObjectToAffectRef) {
		return;
	}

	ObjectToAffect = TScriptInterface<IEventListener>(ObjectToAffectRef);
#ifdef UE_BUILD_DEVELOPMENT
	if (!ObjectToAffect) {
		UE_LOG(LogTemp, Warning,
		       TEXT("EventListener Unensured, Uinsured and Unassured. Please check reference or call programmers! %s"),
		       *GetActorLabel())
	}
#endif
}

void ALongLostLightningCharacter::Tick(float DeltaTime) {
	//Runs every frame
	Super::Tick(DeltaTime);
	//GetLocalViewingPlayerController()->InputEnabled()
	const FRotator CameraRot = GetFollowCamera()->GetComponentRotation();
	const FRotator InverseCharRot = GetActorRotation() * -1;
	FRotator InterpRot = CameraRot + InverseCharRot;
	FRotator EndRotation = FRotator(InterpRot.Roll, InterpRot.Pitch, InterpRot.Yaw + 90);
	if (GetController()) {
		Cast<APlayerController>(GetController())->SetAudioListenerOverride(
			this->GetMesh(),
			FVector::Zero(),
			EndRotation);
	}

	if (Invincible) {
		InvincibleTimer += DeltaTime;
		if (InvincibleTimer > InvincibleCooldown) {
			InvincibleTimer = 0;
			Invincible = false;
			Conductivity = CalculateConductivity();
		}
	}

	DodgeController();
	if (Grabbing) {
		SurfaceGrabbingActor();
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ALongLostLightningCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ALongLostLightningCharacter::Dodge);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &ALongLostLightningCharacter::Move);

		//Interacting
		//EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ALongLostLightningCharacter::Grab);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &ALongLostLightningCharacter::Look);
	}
}

void ALongLostLightningCharacter::SurfaceGrabbingActor() {
	FVector ActorLoc = ActorGrabbing->GetActorLocation();
	FVector TraceStart = ActorLoc + FVector::UpVector * 100;
	FVector TraceEnd = ActorLoc + FVector::DownVector * 1000;
	FHitResult GroundedHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ActorGrabbing);
	QueryParams.AddIgnoredActor(this);

	ActorGrabbing->GetWorld()->LineTraceSingleByChannel(GroundedHit, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);

	FVector GroundNormal = GroundedHit.Normal;
	FVector GroundLoc = GroundedHit.Location;

	ActorGrabbing->SetActorLocation(FVector(ActorLoc.X, ActorLoc.Y, GroundLoc.Z));

	FVector UpVector = ActorGrabbing->GetActorUpVector();

	FVector RotationAxis = FVector::CrossProduct(UpVector, GroundNormal);
	RotationAxis.Normalize();

	float DotProduct = FVector::DotProduct(UpVector, GroundNormal);
	float RotationAngle = acosf(DotProduct);

	FQuat Quat = FQuat(RotationAxis, RotationAngle);
	FQuat RootQuat = ActorGrabbing->GetActorQuat();

	FQuat NewQuat = Quat * RootQuat;

	FRotator QuatRot = NewQuat.Rotator();
	FRotator MyRot = ActorGrabbing->GetActorRotation();
	FRotator NewRot = FMath::Lerp(MyRot, QuatRot, 0.1f);

	ActorGrabbing->SetActorRotation(NewRot);
}

void ALongLostLightningCharacter::Move(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && CanMove && !IsDead) {
		// find out which way is forward
		if (!Grabbing) {
			// MoveRotation = Controller->GetControlRotation();
			MoveRotation = FMath::Lerp(MoveRotation, Controller->GetControlRotation(), DirectionRotationRate);
		}
		const FRotator YawRotation(0, MoveRotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		if (!Grabbing) {
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
			return;
		}

		if (!ActorGrabbing) {
			return;
		}

		const FVector CameraLocation = GetFollowCamera()->GetComponentLocation();
		const FVector CharLocation = GetActorLocation();
		FVector RelativePosition = CameraLocation - CharLocation;
		RelativePosition.Z = 0.0f;
		RelativePosition.Normalize();

		const FRotator GrabbingActorRotation = ActorGrabbing->GetActorRotation();
		const FVector BoxForward = ActorGrabbing->GetActorForwardVector().GetSafeNormal();
		const FVector BoxRightDirection = GrabbingActorRotation.RotateVector(FVector::RightVector).GetSafeNormal();

		const float DotProductRight = FVector::DotProduct(RelativePosition, BoxRightDirection);
		const float DotProductFront = FVector::DotProduct(RelativePosition, BoxForward);
		//FVector FinalMovement;

		// if (DotProductRight > 0.5f) {
		// 	// Object is on the left side of the actor
		// 	FinalMovement = FVector(MovementVector.X, -MovementVector.Y, 0.0f);
		// }
		// else if (DotProductRight < -0.5f) {
		// 	// Object is on the right side of the actor
		// 	FinalMovement = FVector(-MovementVector.X, MovementVector.Y, 0.0f);
		// }
		// else if (DotProductFront > 0.5f) {
		// 	// Object is in front of the actor
		// 	FinalMovement = FVector(-MovementVector.Y, -MovementVector.X, 0.0f);
		// }
		// else if (DotProductFront < -0.5f) {
		// 	// Object is behind the actor
		// 	FinalMovement = FVector(MovementVector.Y, MovementVector.X, 0.0f);
		// }

		//FinalMovement.Normalize();
		AddMovementInput(FollowCamera->GetForwardVector(), MovementVector.Y);
		AddMovementInput(FollowCamera->GetRightVector(), MovementVector.X);
	}
}

void ALongLostLightningCharacter::Dodge() {
	if (CurrentDodgeCooldown <= 0 && !Grabbing && !Dodging) {
		// find out which way is forward
		const FRotator Rotation = this->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		LaunchCharacter(ForwardDirection * DodgeStrength, true, true);
		OnDodge();

		CanMove = false;
		Dodging = true;
		CurrentDodgeCooldown = DodgeCooldown;
	}
}

void ALongLostLightningCharacter::StopDodge() {
	FVector vel = GetVelocity();
	float CurrentSpeed = vel.Size();

	if (CurrentSpeed <= 0) {
		Dodging = false;
		GetWorld()->GetTimerManager().SetTimer(DodgeRecoveryHandle, this,
		                                       &ALongLostLightningCharacter::DodgeRecoveryFrames, RecoveryTime, false);
	}
}

void ALongLostLightningCharacter::DodgeRecoveryFrames() {
	CanMove = true;
}


void ALongLostLightningCharacter::DodgeController() {
	if (CurrentDodgeCooldown > 0) {
		CurrentDodgeCooldown -= 1 * GetWorld()->DeltaRealTimeSeconds;
	}

	if (Dodging) {
		StopDodge();
	}
}


void ALongLostLightningCharacter::Look(const FInputActionValue& Value) {
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * SensitivityXAxis);
		AddControllerPitchInput(LookAxisVector.Y * SensitivityYAxis);
	}
}

AActor* ALongLostLightningCharacter::Grab(AActor* InteractableActor) {
	if (!Grabbing) {
		if (InteractableActor != nullptr) {
			//Get rotation
			FVector PlayerLoc = this->GetActorLocation();
			FVector TargetLoc = InteractableActor->GetActorLocation();
			FVector DesiredLoc = FVector(TargetLoc.X, TargetLoc.Y, PlayerLoc.Z);
			FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(PlayerLoc, DesiredLoc);

			this->SetActorRotation(NewRot);

			//Attach Actor to parent
			const FName fnWeaponSocket = TEXT("PickupSocket");
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(
				EAttachmentRule::KeepWorld,
				EAttachmentRule::KeepWorld,
				EAttachmentRule::KeepWorld,
				false
			);
			InteractableActor->AttachToComponent(GetMesh(), Rules, fnWeaponSocket);
			Cast<IInteractableInterface>(InteractableActor)->AttachedToPlayer = true;

			//Set grabbing state to true
			Grabbing = true;
			MoveRotation = this->GetActorRotation();
			ActorGrabbing = InteractableActor;
			Cast<IInteractableInterface>(InteractableActor)->OnInteract();
		}
	}
	else {
		StopGrab();
	}
	return ActorGrabbing;
}

void ALongLostLightningCharacter::StopGrab() {
	Cast<IInteractableInterface>(ActorGrabbing)->AttachedToPlayer = false;
	ActorGrabbing->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	Grabbing = false;
}

AActor* ALongLostLightningCharacter::GetClosestInteractable() {
	if (InteractableArray.Num() == 0) {
		return nullptr;
	}
	if (InteractableArray.Num() > 1) {
		InteractableArray.Sort([this](const AActor& a, const AActor& b) {
			return a.GetDistanceTo(this) > b.GetDistanceTo(this);
		});
	}
	return InteractableArray[0];
}

void ALongLostLightningCharacter::AddInteractableToArray(AActor* OverlappedObject) {
	if (OverlappedObject->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass())) {
		InteractableArray.Add(OverlappedObject);
	}
}

void ALongLostLightningCharacter::RemoveInteractableFromArray(AActor* OverlappedObject) {
	if (OverlappedObject->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass())) {
		InteractableArray.Remove(OverlappedObject);
	}
}
