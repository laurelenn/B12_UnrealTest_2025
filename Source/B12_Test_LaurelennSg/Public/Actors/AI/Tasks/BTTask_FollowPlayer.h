// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FollowPlayer.generated.h"

/**
 * 
 */
UCLASS()
class B12_TEST_LAURELENNSG_API UBTTask_FollowPlayer : public UBTTaskNode
{
	GENERATED_BODY()

	public : 
		UBTTask_FollowPlayer();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	/**Will rotate to face placer !*/
	UPROPERTY(EditAnywhere, Category = "#Setup")
		bool bFacePlayer = true;

	UPROPERTY(Category = "Runtime", VisibleAnywhere)
	FVector TargetLocation;
};
