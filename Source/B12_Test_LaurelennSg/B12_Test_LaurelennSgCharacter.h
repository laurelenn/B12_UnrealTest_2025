// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Enums.h"
#include "B12_Test_LaurelennSgCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInteractionComponent;
class UHoldableComponent;
class UCaptureGameManager;
class ULifeComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AB12_Test_LaurelennSgCharacter : public ACharacter
{
	GENERATED_BODY()

	#pragma region Default Unreal Template
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayingMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/**Interact Input Action -- Call to interact with interactable components*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** GameState Start - Call for start and restart*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GameStartAction;

public:
	AB12_Test_LaurelennSgCharacter();
	

protected:

	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	#pragma endregion

	////////////////////// CUSTOM CODE /////////////////////



	/**Distance to drop the hold actor*/
	UPROPERTY(Category = "#Setup", EditDefaultsOnly, BlueprintReadWrite)
	float DropDistance = 100.f;

	UPROPERTY(Category = "#Setup", EditDefaultsOnly, BlueprintReadWrite)
	FName ParentSocketHoldName;

	UPROPERTY(Category = "Components", EditDefaultsOnly, BlueprintReadOnly)
	ULifeComponent* LifeComponent;

	/**Socket where attach the Actor Hold when exists*/
	UPROPERTY(Category = "Components", EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* SocketHold;

	/**InteractionComponent the player can interact with*/
	UPROPERTY(Category = Runtime, VisibleAnywhere, BlueprintReadOnly)
	UInteractionComponent* TriggeableInteractComp;

	/**Current Holdable Component hold in hand*/
	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	UHoldableComponent* HeldComponent;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	APlayerController* PlayerController;


	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	UCaptureGameManager* GameManager;
	
	public : 
	UFUNCTION(BlueprintCallable)
	bool TryHoldActor(UHoldableComponent* CompToHold);

	UFUNCTION(BlueprintCallable)
	bool TryDropActorHold(UHoldableComponent* CompToHold);

	/**Function call with the Interact Input Action*/
	UFUNCTION(BlueprintCallable)
	void Interact();

	/**Function call with the GameStart Input Action*/
	UFUNCTION(BlueprintCallable)
	void CallGameStart();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LaunchDeath(ULifeComponent* LifeComp, AActor* Causer);

	UFUNCTION()
	void OnGameStateChange(ECaptureGameState gameState);

};

