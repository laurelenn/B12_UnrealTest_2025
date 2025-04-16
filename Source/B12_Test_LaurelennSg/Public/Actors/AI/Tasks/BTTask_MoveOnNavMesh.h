// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveOnNavMesh.generated.h"

UCLASS()
class B12_TEST_LAURELENNSG_API UBTTask_MoveOnNavMesh : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveOnNavMesh();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	/**A distance the AI can walk to*/
	UPROPERTY(EditAnywhere, Category = "#Setup")
	float WalkDistanceSelection = 500.f;

	FVector TargetLocation = FVector::ZeroVector;
};