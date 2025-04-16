// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Tasks/BTService_CheckPlayerDistance.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/AI/AIPreyBase.h"
#include "DataAssets/AI/AIPreyDataAsset.h"

UBTService_CheckPlayerDistance::UBTService_CheckPlayerDistance()
{
}

void UBTService_CheckPlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIPreyController* Controller = Cast<AAIPreyController>(OwnerComp.GetAIOwner());
	AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject(Controller->Key_PlayerActor));
	AAIPreyBase* AIPrey = Cast<AAIPreyBase>(Controller->GetPawn());

	if (!Player || !AIPrey || !Controller || !Blackboard || !AIPrey->DataAsset)
	{
		return;
	}

	const float Distance = FVector::Dist(Player->GetActorLocation(), AIPrey->GetActorLocation());
	const float StartFleeDistance = AIPrey->DataAsset->DistancePlayerDetection;
	const float FleeSafeDistance = AIPrey->DataAsset->SafeDistance;

	uint8 StateValue = Blackboard->GetValueAsEnum(Controller->Key_AIPreyState);
	const EAIPreyState CurrentState = static_cast<EAIPreyState>(StateValue);

	if (CurrentState == EAIPreyState::Chilling && Distance <= StartFleeDistance)
	{
		Controller->SetAIPreyState(EAIPreyState::Flighting);
	}
	else if (CurrentState == EAIPreyState::Flighting && Distance > FleeSafeDistance)
	{
		Controller->SetAIPreyState(EAIPreyState::Chilling);
	}
}
