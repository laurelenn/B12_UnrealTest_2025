// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Tasks/BTTask_CheckPlayerDistAndFlee.h"

UBTTask_CheckPlayerDistAndFlee::UBTTask_CheckPlayerDistAndFlee()
{

	NodeName = TEXT("Check Player Distance and Flee");

}

EBTNodeResult::Type UBTTask_CheckPlayerDistAndFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject("PlayerActor"));
	AAIPreyController* Controller = Cast<AAIPreyController>(OwnerComp.GetAIOwner());
	AAIPreyBase* AIPrey = Cast<AAIPreyBase>(Controller->GetPawn());

	if (!Player || !AIPrey || !Controller || !Blackboard || !AIPrey->DataAsset)
	{
		return EBTNodeResult::Failed;
	}

	float Distance = FVector::Dist(Player->GetActorLocation(), AIPrey->GetActorLocation());

	if (Distance <= AIPrey->DataAsset->DistancePlayerDetection)
	{
		Blackboard->SetValueAsEnum(Controller->Key_AIPreyState, static_cast<uint8>(EAIPreyState::Flighting));
	}

	return EBTNodeResult::Succeeded;
}
