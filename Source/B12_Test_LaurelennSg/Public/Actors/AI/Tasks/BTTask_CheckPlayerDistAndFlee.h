// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckPlayerDistAndFlee.generated.h"

/**
 * 
 */
UCLASS()
class B12_TEST_LAURELENNSG_API UBTTask_CheckPlayerDistAndFlee : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_CheckPlayerDistAndFlee();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
