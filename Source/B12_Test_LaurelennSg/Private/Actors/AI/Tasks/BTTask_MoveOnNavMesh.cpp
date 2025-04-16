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
	AAIPreyBase* AIPrey = Cast<AAIPreyBase>(AIController->GetCharacter());
	UWorld* World = AIPrey->GetWorld();
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);


	if (!AIController || !World || !NavSystem || !AIPrey)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTTask_MoveOnNavMesh::ExecuteTask : Something is invalid !"));

		return EBTNodeResult::Failed;
	}

	
	


	if (AIPrey->DataAsset)
	{
		FNavLocation RandomLocation;
		bool bFound = NavSystem->GetRandomReachablePointInRadius(AIPrey->GetActorLocation(), WalkDistanceSelection, RandomLocation);
		DrawDebugSphere(GetWorld(), AIPrey->GetActorLocation(), 50.f, 12, FColor::Red, false, 2.f);
		DrawDebugSphere(GetWorld(), RandomLocation.Location, 50.f, 12, FColor::Green, false, 2.f);
		if (bFound) 
		{
			// Set speed from DataAsset
			AIPrey->GetCharacterMovement()->MaxWalkSpeed = AIPrey->DataAsset->SpeedWalk;
	
			// Adjust height to make sure mesh touches the ground
			const FVector MeshOffset = AIPrey->GetMesh()->Bounds.BoxExtent;
			FVector TargetLocation = RandomLocation.Location;
			TargetLocation.Z += MeshOffset.Z;

			AIController->MoveToLocation(TargetLocation);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UBTTask_MoveOnNavMesh::ExecuteTask : No reachable point found !"));
			const ANavigationData* NavData = UNavigationSystemV1::GetCurrent(GetWorld())->GetNavDataForProps(AIPrey->GetNavAgentPropertiesRef());

			if (NavData)
			{
				UE_LOG(LogTemp, Log, TEXT("UBTTask_MoveOnNavMesh::ExecuteTask : Using NavData: %s"), *NavData->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UBTTask_MoveOnNavMesh::ExecuteTask : No matching NavData found for this agent."));
			}
			return EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}