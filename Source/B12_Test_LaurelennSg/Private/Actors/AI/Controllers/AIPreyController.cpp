// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Controllers/AIPreyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Actors/AI/AIPreyBase.h"
#include <Kismet/GameplayStatics.h>
#include "ActorComponents/HoldableComponent.h"
#include "Managers/CaptureGameManager.h"
#include "ToolboxBPFL.h"

AAIPreyController::AAIPreyController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAIPreyController::BeginPlay()
{
	Super::BeginPlay();

	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();
	if (GameManager)
	{
		GameManager->OnGameStateChange.AddDynamic(this, &AAIPreyController::OnGameStateChange);
		if (GameManager->GetGameState() == ECaptureGameState::Playing)
		{
			SetAIActive(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AAIPreyController::BeginPlay : No Game Manager found !!"));
	}

	if (BehaviorTreeAsset)
	{
		if (BehaviorTreeAsset->BlackboardAsset)
		{
			UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
			RunBehaviorTree(BehaviorTreeAsset);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AIPreyController::BeginPlay : BlackboardAsset is not set!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AIPreyController::BeginPlay : BehaviorTreeAsset is set not set !"));

	}
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
	else if (BlackboardComp && BlackboardComp->GetValueAsEnum(Key_AIPreyState) != static_cast<uint8>(EAIPreyState::Captured)) // If captured don't make it idle again !
	{
		SetAIPreyState(EAIPreyState::Idle);
	}
}

void AAIPreyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Assign player "target"
	SetPlayerActor(Player);

	SetAIPreyState(EAIPreyState::Idle); // Could be latent with a delay activation too

	// Bind to events
	AAIPreyBase* AIPreyPawn = Cast<AAIPreyBase>(InPawn);
	if (IsValid(AIPreyPawn))
	{
		AIPreyPawn->OnCaptured.AddDynamic(this, &AAIPreyController::HandleAIPawnCaptured);
		AIPreyPawn->GetHoldComp()->OnHeld.AddDynamic(this, &AAIPreyController::HandleAIPawnHeld);
		UE_LOG(LogTemp, Log, TEXT("AIPreyController::OnPossess : Subscribed to capture and held events !"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AIPreyController::OnPossess : AIPreyPawn is not valid !"));
	}
}

void AAIPreyController::SetAIPreyState(EAIPreyState NewState)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(Key_AIPreyState, static_cast<uint8>(NewState));
	}
}

void AAIPreyController::SetPlayerActor(AActor* PlayerActor)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(Key_PlayerActor, PlayerActor);
	}
}

void AAIPreyController::SetAIActive(bool bActive)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(Key_AIActive, bActive);
	}
}

void AAIPreyController::OnGameStateChange(ECaptureGameState GameState)
{
	switch (GameState)
	{
		case ECaptureGameState::Playing : 
			if (BlackboardComp && BlackboardComp->GetValueAsObject(Key_PlayerActor) == NULL)
			{
				AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Assign player "target"
				SetPlayerActor(Player);
			}
			SetAIActive(true);
			break;

		default:
			SetAIActive(false);
			break;
	}
}
