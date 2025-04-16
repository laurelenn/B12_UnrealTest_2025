// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Tasks/BTTask_FleeOnNavMesh.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/AI/AIPreyBase.h"
#include "DataAssets/AI/AIPreyDataAsset.h"

UBTTask_FleeOnNavMesh::UBTTask_FleeOnNavMesh()
{
	NodeName = TEXT("Flee");
}

EBTNodeResult::Type UBTTask_FleeOnNavMesh::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIPreyController* Controller = Cast<AAIPreyController>(OwnerComp.GetAIOwner());
	AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject(Controller->Key_PlayerActor));
	AAIPreyBase* AIPrey = Cast<AAIPreyBase>(Controller->GetPawn());
	UWorld* World = AIPrey->GetWorld();
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);


	if (!Controller || !World || !NavSystem || !AIPrey || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector PreyLocation = AIPrey->GetActorLocation();
	const FVector FleeDirection = (PreyLocation - PlayerLocation).GetSafeNormal();

	if (AIPrey->DataAsset)
	{
		const FVector FleeTargetLocation = PreyLocation + FleeDirection * FleeDistanceSelection; // need to know to have the good direction
		
		FNavLocation RandomLocation;
		const bool bFound = NavSystem->GetRandomPointInNavigableRadius(FleeTargetLocation, 100.f, RandomLocation); // Need to know the direction
		
		if (bFound)
		{
			// Set speed from DataAsset
			AIPrey->GetCharacterMovement()->MaxWalkSpeed = AIPrey->DataAsset->SpeedFlight;

			// Adjust height to make sure mesh touches the ground
			const FVector MeshOffset = AIPrey->GetMesh()->Bounds.BoxExtent;
			FVector TargetLocation = RandomLocation.Location;
			TargetLocation.Z += MeshOffset.Z;

			Controller->MoveToLocation(TargetLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
