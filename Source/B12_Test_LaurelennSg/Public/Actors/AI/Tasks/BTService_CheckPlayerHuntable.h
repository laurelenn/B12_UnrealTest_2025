// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerHuntable.generated.h"

/**
 * 
 */
UCLASS()
class B12_TEST_LAURELENNSG_API UBTService_CheckPlayerHuntable : public UBTService
{
	GENERATED_BODY()

	public : 
		UBTService_CheckPlayerHuntable();

	protected : 
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
