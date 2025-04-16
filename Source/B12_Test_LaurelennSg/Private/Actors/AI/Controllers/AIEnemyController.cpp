// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/Controllers/AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Actors/AI/AIEnemyBase.h"
#include <Kismet/GameplayStatics.h>
#include "ActorComponents/ProjectileBalLauncher.h"
#include "Managers/CaptureGameManager.h"
#include "ToolboxBPFL.h"

AAIEnemyController::AAIEnemyController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();
	if (GameManager)
	{
		GameManager->OnGameStateChange.AddDynamic(this, &AAIEnemyController::OnGameStateChange);
		if (GameManager->GetGameState() == ECaptureGameState::Playing)
		{
			SetAIActive(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AAIEnemyController::BeginPlay : No Game Manager found !!"));
	}
	AIEnemyPawn = Cast<AAIEnemyBase>(GetOwner());

	if (BehaviorTreeAsset)
	{
		if (BehaviorTreeAsset->BlackboardAsset)
		{
			UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
			RunBehaviorTree(BehaviorTreeAsset);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AAIEnemyController::BeginPlay : BlackboardAsset is not set!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AAIEnemyController::BeginPlay : BehaviorTreeAsset is set not set !"));

	}
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Assign player "target"
	SetPlayerActor(Player);

	//SetAIEnemyState(EAIEnemyState::Idle); // Could be latent with a delay activation too
}

void AAIEnemyController::SetAIEnemyState(EAIEnemyState NewState)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(Key_AIEnemyState, static_cast<uint8>(NewState));
		if (IsValid(AIEnemyPawn))
		{
			if (NewState == EAIEnemyState::Hunting && BlackboardComp->GetValueAsBool(Key_AIActive))
			{
				AIEnemyPawn->GetProjectileLauncherComp()->SetEnabled(true); // Activate if hunting and active
			}
		}
	}
}

void AAIEnemyController::SetPlayerActor(AActor* PlayerActor)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(Key_PlayerActor, PlayerActor);
	}
}

void AAIEnemyController::SetAIActive(bool bActive)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(Key_AIActive,bActive);
	}

	if (IsValid(AIEnemyPawn))
	{
		AIEnemyPawn->GetProjectileLauncherComp()->SetEnabled(bActive && BlackboardComp->GetValueAsEnum(Key_AIEnemyState) != static_cast<uint8>(EAIEnemyState::Hunting));  // Activate if hunting and active
	}
}

void AAIEnemyController::OnGameStateChange(ECaptureGameState GameState)
{
	switch (GameState)
	{
	case ECaptureGameState::Playing:
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
