// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class B12_TEST_LAURELENNSG_API UBTService_CheckPlayerDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckPlayerDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
