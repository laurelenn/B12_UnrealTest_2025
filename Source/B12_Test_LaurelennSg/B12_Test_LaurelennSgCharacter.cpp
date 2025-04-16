// Copyright Epic Games, Inc. All Rights Reserved.

#include "B12_Test_LaurelennSgCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ActorComponents/InteractionComponent.h"
#include "ActorComponents/HoldableComponent.h"
#include "ActorComponents/LifeComponent.h"
#include "Managers/CaptureGameManager.h"
#include "ToolboxBPFL.h"

/* ***** NOTE *****
This Code is generated automatically by Unreal Third Caracter Template for the main part
******************* */

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AB12_Test_LaurelennSgCharacter

AB12_Test_LaurelennSgCharacter::AB12_Test_LaurelennSgCharacter()
{
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
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	LifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("LifeComponent"));

	SocketHold = CreateDefaultSubobject<USceneComponent>(TEXT("SocketHold"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AB12_Test_LaurelennSgCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (GameManager)
			{
				ECaptureGameState CurrentState = GameManager->GetGameState();
				
				Subsystem->AddMappingContext(CurrentState==ECaptureGameState::Playing ? PlayingMappingContext : DefaultMappingContext, 0);
			}
		}
	}
}

void AB12_Test_LaurelennSgCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AB12_Test_LaurelennSgCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AB12_Test_LaurelennSgCharacter::Look);
	
		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AB12_Test_LaurelennSgCharacter::Interact);
		
		// Start/Restart Game
		EnhancedInputComponent->BindAction(GameStartAction, ETriggerEvent::Started, this, &AB12_Test_LaurelennSgCharacter::CallGameStart);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



void AB12_Test_LaurelennSgCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameManager = UToolboxBPFL::GetGameManager();

	if (GameManager)
	{
		GameManager->RegisterPlayer(this);
		GameManager->OnGameStateChange.AddDynamic(this, &AB12_Test_LaurelennSgCharacter::OnGameStateChange);
	}

	if (IsValid(LifeComponent))
	{
		LifeComponent->OnDeath.AddDynamic(this, &AB12_Test_LaurelennSgCharacter::LaunchDeath);
	}

	SocketHold->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,ParentSocketHoldName);

	FName AttachedSocket = SocketHold->GetAttachSocketName();
	UE_LOG(LogTemp, Warning, TEXT("AB12_Test_LaurelennSgCharacter::BeginPlay : SocketHold is attached to socket: %s"), *AttachedSocket.ToString());

}

void AB12_Test_LaurelennSgCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AB12_Test_LaurelennSgCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


bool AB12_Test_LaurelennSgCharacter::TryHoldActor(UHoldableComponent* CompToHold)
{
	// Check if CompToHold is valid, and if there is not another actorHold, and if isn't already Hold
	if (CompToHold && !HeldComponent && CompToHold != HeldComponent && CompToHold->bIsEnabled)
	{
		HeldComponent = CompToHold;
		HeldComponent->HandleHoldDrop(true);
		HeldComponent->GetOwner()->AttachToComponent(SocketHold, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		return true;
	}
	return false;
}

bool AB12_Test_LaurelennSgCharacter::TryDropActorHeld(UHoldableComponent* CompToHold)
{
	// Check if there is an actor Hold and if it's not the CompToHold's Owner
	if (HeldComponent && HeldComponent == CompToHold)
	{
		AActor* HeldActor = HeldComponent->GetOwner();
		HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// Calculation - Location to drop the actor --> Make a line trace to get the floor
		FVector StartLocation = GetActorLocation() + GetActorForwardVector() * DropDistance;
		FVector EndLocation = StartLocation - FVector(0.f, 0.f, 1000.f);

		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(HeldActor);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			TraceParams
		);

		if (bHit)
		{
			FVector DropLocation = HitResult.Location;
			UPrimitiveComponent* MeshComp = HeldActor->FindComponentByClass<UPrimitiveComponent>();
			if (MeshComp) // To make the mesh actor drop on floor, ignore the collisions
			{
				float MeshOffsetZ = MeshComp->Bounds.BoxExtent.Z;
				DropLocation.Z += MeshOffsetZ;
			}
			HeldActor->SetActorLocation(DropLocation);
		}
		else
		{
			// If line trace not hit, we use the player Z
			FVector DropLocation = StartLocation;
			DropLocation.Z = GetActorLocation().Z;
			HeldActor->SetActorLocation(DropLocation);
		}

		// Reinit rotation
		FRotator CurrentRot = HeldActor->GetActorRotation();
		FRotator DropRotation(0.f, CurrentRot.Yaw, 0.f);

		HeldActor->SetActorRotation(DropRotation);

		HeldComponent->HandleHoldDrop(false);
		HeldComponent = nullptr;
		TriggeableInteractComp = nullptr;

		return true;
	}
	return false;
}

void AB12_Test_LaurelennSgCharacter::Interact()
{
	if (TriggeableInteractComp)
	{
		TriggeableInteractComp->TryInteractWith();
	}
}

void AB12_Test_LaurelennSgCharacter::CallGameStart()
{
	if (GameManager)
	{
		ECaptureGameState CurrentState = GameManager->GetGameState();

		switch (CurrentState)
		{
			case ECaptureGameState::ReadyToStart :
			GameManager->SetGameState(ECaptureGameState::Playing);
			break;

			case ECaptureGameState::ReadyToRestart :
			GameManager->SetGameState(ECaptureGameState::Initialization);
			break;

			default :
			break;
		}
	}

}

void AB12_Test_LaurelennSgCharacter::LaunchDeath_Implementation(ULifeComponent* LifeComp, AActor* Causer)
{
	// Custom in bp for animation !
}

void AB12_Test_LaurelennSgCharacter::OnGameStateChange(ECaptureGameState gameState)
{
	if (GameManager && PlayerController) // Check but should already exist
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			ECaptureGameState CurrentState = GameManager->GetGameState();
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(CurrentState == ECaptureGameState::Playing ? PlayingMappingContext : DefaultMappingContext, 0);
		}
	}

}
