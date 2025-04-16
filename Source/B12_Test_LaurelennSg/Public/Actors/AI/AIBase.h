// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "AIBase.generated.h"

class ULifeComponent;
class UCapsuleComponent;
class UArrowComponent;

// Just a base for All AI
UCLASS(Abstract)
class B12_TEST_LAURELENNSG_API AAIBase : public ACharacter
{
	GENERATED_BODY()


#pragma region ----- VARIABLES -----

protected : 

	UPROPERTY(Category = "Components", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ULifeComponent* LifeComponent;

	UPROPERTY(Category = Runtime , VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bIsActive = false;

#pragma endregion

#pragma region ----- FUNCTIONS -----


public:
	// Sets default values for this pawn's properties
	AAIBase();

protected :
	virtual void BeginPlay() override;

	virtual void SetIsActive(bool IsActive){bIsActive = IsActive;}


	UFUNCTION()
	virtual void OnGameStateChange(ECaptureGameState GameState);
#pragma endregion


};
