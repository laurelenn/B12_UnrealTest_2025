// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Controllers/AIPreyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Actors/AI/AIPreyBase.h"
#include <Kismet/GameplayStatics.h>

AAIPreyController::AAIPreyController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAIPreyController::BeginPlay()
{
	Super::BeginPlay();
}

void AAIPreyController::HandleAIPawnCaptured(AAIPreyBase* AIPrey)
{
	SetAIPreyState(EAIPreyState::Captured);
}

void AAIPreyController::HandleAIPawnHeld(UHoldableComponent* HeldComponent, bool bIsHeld)
{
	if (bIsHeld)
	{
		SetAIPreyState(EAIPreyState::Held);
	}
	else
	{
		SetAIPreyState(EAIPreyState::Chilling);
	}
}

void AAIPreyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset && BehaviorTreeAsset->BlackboardAsset)
	{
		UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
		RunBehaviorTree(BehaviorTreeAsset);

		AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Assign player "target"
		SetPlayerActor(Player);

		SetAIPreyState(EAIPreyState::Chilling); // Could be latent with a delay activation too

		// Bind to events
		AAIPreyBase* AIPreyPawn = Cast<AAIPreyBase>(InPawn);
		if (IsValid(AIPreyPawn))
		{
			AIPreyPawn->OnCaptured.AddDynamic(this, &AAIPreyController::HandleAIPawnCaptured);
			AIPreyPawn->GetHoldComp()->OnHeld.AddDynamic(this, &AAIPreyController::HandleAIPawnHeld);
		}
	}
}

void AAIPreyController::SetAIPreyState(EAIPreyState NewState)
{
	BlackboardComp->SetValueAsEnum(Key_AIPreyState, static_cast<uint8>(NewState));
}

void AAIPreyController::SetPlayerActor(AActor* PlayerActor)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(Key_PlayerActor, PlayerActor);
	}
}
