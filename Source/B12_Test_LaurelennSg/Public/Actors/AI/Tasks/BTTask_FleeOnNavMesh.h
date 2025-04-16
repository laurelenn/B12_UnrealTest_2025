// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FleeOnNavMesh.generated.h"

/**
 * 
 */
UCLASS()
class B12_TEST_LAURELENNSG_API UBTTask_FleeOnNavMesh : public UBTTaskNode
{
	GENERATED_BODY()
	
public : 
	UBTTask_FleeOnNavMesh();

protected : 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	/**A distance the AI can flee to*/
	UPROPERTY(EditAnywhere, Category = "#Setup")
	float FleeDistanceSelection = 500.f;

};
