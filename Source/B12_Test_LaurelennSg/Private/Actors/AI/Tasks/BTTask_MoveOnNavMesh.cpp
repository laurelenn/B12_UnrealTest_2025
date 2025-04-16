// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Tasks/BTTask_MoveOnNavMesh.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/AI/AIPreyBase.h"
#include "DataAssets/AI/AIPreyDataAsset.h"

UBTTask_MoveOnNavMesh::UBTTask_MoveOnNavMesh()
{
	NodeName = "Move Around NavMesh";
}

EBTNodeResult::Type UBTTask_MoveOnNavMesh::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	ACharacter* AICharacter = AIController->GetCharacter();
	UWorld* World = AICharacter->GetWorld();
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	AAIPreyBase* AIPrey = Cast<AAIPreyBase>(AICharacter);

	if (!AIController || !AICharacter || !World || !NavSystem || !AIPrey)
	{
		return EBTNodeResult::Failed;
	}

	if (AIPrey->DataAsset)
	{
		FNavLocation RandomLocation;
		bool bFound = NavSystem->GetRandomReachablePointInRadius(AICharacter->GetActorLocation(), WalkDistanceSelection, RandomLocation);
		if (bFound) 
		{
			// Set speed from DataAsset
			AIPrey->GetCharacterMovement()->MaxWalkSpeed = AIPrey->DataAsset->SpeedWalk;
	
			// Adjust height to make sure mesh touches the ground
			const FVector MeshOffset = AICharacter->GetMesh()->Bounds.BoxExtent;
			FVector TargetLocation = RandomLocation.Location;
			TargetLocation.Z += MeshOffset.Z;

			AIController->MoveToLocation(TargetLocation);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}