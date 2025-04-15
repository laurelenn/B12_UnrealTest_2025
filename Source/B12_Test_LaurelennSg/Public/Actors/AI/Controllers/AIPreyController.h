// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIPreyController.generated.h"

/**
 * 
 */
UCLASS()
class B12_TEST_LAURELENNSG_API AAIPreyController : public AAIController
{
	GENERATED_BODY()
	
#pragma region ----- VARIABLES -----	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "#Setup")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "#Setup")
	UBlackboardComponent* BlackboardComp;

	/** Keys */
	UPROPERTY(EditDefaultsOnly, Category = "#Setup|Blackboard")
	FName Key_AIPreyState = "AIPreyState";

	UPROPERTY(EditDefaultsOnly, Category = "#Setup|Blackboard")
	FName Key_PlayerActor = "PlayerActor";

#pragma endregion

#pragma region ----- FUNCTIONS -----

public:
	AAIPreyController();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleAIPawnCaptured(AAIPreyBase* AIPrey);

	void HandleAIPawnHeld(UHoldableComponent* HeldComponent, bool bIsHeld);

public:
	/** Lance le BT si tout est pr�t */
	virtual void OnPossess(APawn* InPawn) override;

	/** Blackboard setters **/
	UFUNCTION(BlueprintCallable)
	void SetAIPreyState(EAIPreyState NewState);

	UFUNCTION()
	void SetPlayerActor(AActor* PlayerActor);
#pragma endregion
};
