// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enums.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class B12_TEST_LAURELENNSG_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	
#pragma region ----- VARIABLES -----	

public:
	UPROPERTY(EditDefaultsOnly, Category = "#Setup")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "#Setup")
	UBlackboardComponent* BlackboardComp;

	/** Keys */
	UPROPERTY(EditDefaultsOnly, Category = "#Setup|Blackboard")
	FName Key_AIEnemyState = "AIEnemyState";

	UPROPERTY(EditDefaultsOnly, Category = "#Setup|Blackboard")
	FName Key_PlayerActor = "PlayerActor";

	UPROPERTY(EditDefaultsOnly, Category = "#Setup|Blackboard")
	FName Key_AIActive = "IsActive";

protected : 

	AAIEnemyBase* AIEnemyPawn;
#pragma endregion

#pragma region ----- FUNCTIONS -----

public:
	AAIEnemyController();

protected:
	virtual void BeginPlay() override;

public:
	/** Lance le BT si tout est prêt */
	virtual void OnPossess(APawn* InPawn) override;

	/** Blackboard setters **/
	UFUNCTION(BlueprintCallable)
	void SetAIEnemyState(EAIEnemyState NewState);

	UFUNCTION()
	void SetPlayerActor(AActor* PlayerActor);

	UFUNCTION()
	void SetAIActive(bool bActive);

	UFUNCTION()
	void OnGameStateChange(ECaptureGameState GameState);
#pragma endregion

};
