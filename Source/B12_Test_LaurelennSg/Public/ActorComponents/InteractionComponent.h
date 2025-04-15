// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Enums.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActivationWithParams, bool, HasAlreadyBeenActivated, AB12_Test_LaurelennSgCharacter*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeactivation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeactivationWithParams, bool, HasAlreadyBeenDeactivated, AB12_Test_LaurelennSgCharacter*, Player);


class UWidgetComponent;
class AB12_Test_LaurelennSgCharacter;

/**
 * Will be activated/deactivated with the interaction input of the player. Can be attached to anything
 */
UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()
	
	public : 

	#pragma region ----- VARIABLES -----

	/**If true - will have a "deactivation" behaviour when the player has already activated it*/
	UPROPERTY(Category = "#Setup", EditAnywhere)
	bool bCanBeDeactivated = true;

	/**Set it if you don't want to make it activable anymore*/
	UPROPERTY(Category = "#Setup", EditAnywhere)
	bool bIsEnabled = true;

	/**Widget to show when the player is close enough*/
	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetclassToShow;

	/**Location offset of the widget relative to the owner location*/
	UPROPERTY(Category = "#Setup", EditAnywhere)
	FVector WidgetOffset = FVector::ZeroVector;

	UPROPERTY(Category = "#Setup", EditAnywhere)
	FVector WidgetScale = FVector::OneVector;


	/**True if the player is on the detection zone and can interact with*/
	UPROPERTY(Category = "Runtime", VisibleAnywhere)
	bool bIsTriggeable = false;

	UPROPERTY(Category = "Runtime", VisibleAnywhere)
	bool bIsActivated = false;

	/**To Check if it's the first Activation (if we want specific behaviour)*/
	UPROPERTY(Category = "Runtime", VisibleAnywhere)
	bool bHasAlreadyBeenActivated = false;

	/**To Check if it's the first Deactivation (if we want specific behaviour)*/
	UPROPERTY(Category = "Runtime", VisibleAnywhere)
	bool bHasAlreadyBeenDeactivated = false;

	/**The player we're overlapping with*/
	UPROPERTY(Category = "Runtime", VisibleAnywhere)
	AB12_Test_LaurelennSgCharacter* OverlappingPlayer;

	UPROPERTY()
	UWidgetComponent* WidgetComp;

	UPROPERTY()
	UUserWidget* Widget;

	FOnActivation OnActivation;
	FOnActivationWithParams OnActivationWithParams;
	FOnDeactivation OnDeactivation;
	FOnDeactivationWithParams OnDeactivationWithParams;
	#pragma endregion

#pragma region ----- FUNCTIONS -----

	UInteractionComponent();
protected : 

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void UpdateWidgetRotation();
public:

	UFUNCTION(BlueprintCallable)
	virtual void TryInteractWith();

	UFUNCTION(BlueprintCallable)
	virtual void ActivateInteraction();

	UFUNCTION(BlueprintCallable)
	virtual void DeactivateInteraction();

protected : 
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnGameStateChange(ECaptureGameState GameState);

#pragma endregion

};
