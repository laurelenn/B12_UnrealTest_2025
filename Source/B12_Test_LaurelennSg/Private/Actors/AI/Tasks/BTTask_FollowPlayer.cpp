// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Tasks/BTTask_FollowPlayer.h"
#include "Actors/AI/AIEnemyBase.h"
#include "DataAssets/AI/AIEnemyDataAsset.h"
#include "Actors/AI/Controllers/AIEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_FollowPlayer::UBTTask_FollowPlayer()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FollowPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyBase* AIEnemy = Cast<AAIEnemyBase>((OwnerComp.GetAIOwner()->GetPawn()));

	AIEnemy->GetCharacterMovement()->MaxWalkSpeed = AIEnemy->DataAsset->Speed;
	AIEnemy->GetCharacterMovement()->bOrientRotationToMovement = !bFacePlayer;

	return EBTNodeResult::InProgress; // Just start the task
}

void UBTTask_FollowPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIEnemyController* AIController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());
	AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject(AIController->Key_PlayerActor));
	AAIEnemyBase* AIEnemy = Cast<AAIEnemyBase>(AIController->GetPawn());

	if (!Player || !AIEnemy || !AIController || !Blackboard || !AIEnemy->DataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT(" UBTService_CheckPlayerHuntable::TickNode : Something is invalid !")); 
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector AILocation = AIEnemy->GetActorLocation();
	const float CurrentDistance = FVector::Dist(PlayerLocation, AILocation);


	const float MinDistance = AIEnemy->DataAsset->DistanceHuntPlayer;

	// If too close
	if (CurrentDistance < MinDistance)
	{
		const FVector DirectionAway = (AILocation - PlayerLocation).GetSafeNormal();
		TargetLocation = AILocation + DirectionAway * (MinDistance-CurrentDistance);

		AIController->MoveToLocation(TargetLocation, 5.f, false);
	}
	else
	{
		// Follow Player
		const FVector DirectionToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
		TargetLocation = PlayerLocation - DirectionToPlayer * MinDistance;

		AIController->MoveToLocation(TargetLocation, 5.f, false);
	}

	if (bFacePlayer)
	{
		FVector ToPlayerLocation = Player->GetActorLocation() - AIEnemy->GetActorLocation();
		FRotator LookAtRotation = ToPlayerLocation.Rotation();
		AIEnemy->SetActorRotation(FMath::RInterpTo(AIEnemy->GetActorRotation(), LookAtRotation, DeltaSeconds, 5.f));
	}

	// The task never "finish", she will be stoped when we change AIState
}
