// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Tasks/BTService_CheckPlayerHuntable.h"
#include "AIController.h"
#include "Actors/AI/AIEnemyBase.h"
#include "DataAssets/AI/AIEnemyDataAsset.h"
#include "Actors/AI/Controllers/AIEnemyController.h"


UBTService_CheckPlayerHuntable::UBTService_CheckPlayerHuntable()
{
}

void UBTService_CheckPlayerHuntable::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIEnemyController* Controller = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());
	AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject(Controller->Key_PlayerActor));
	AAIEnemyBase* AIEnemy = Cast<AAIEnemyBase>(Controller->GetPawn());

	if (!Player || !AIEnemy || !Controller || !Blackboard || !AIEnemy->DataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT(" UBTService_CheckPlayerHuntable::TickNode : Something is invalid !"));
		return;
	}

	const float DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), AIEnemy->GetActorLocation());
	uint8 currentState = Blackboard->GetValueAsEnum(Controller->Key_AIEnemyState);

	if (DistanceToPlayer <= AIEnemy->DataAsset->MaxDistancePlayerDetect)
	{
		if (currentState != static_cast<uint8>(EAIEnemyState::Hunting)) // we will not notify changed every time
		{
			Controller->SetAIEnemyState(EAIEnemyState::Hunting);
		}
	}
	else
	{
		if (currentState != static_cast<uint8>(EAIEnemyState::Idle)) // we will not notify changed every time
		{
			Controller->SetAIEnemyState(EAIEnemyState::Idle);
		}
	}
}
