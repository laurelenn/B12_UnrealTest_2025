// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/AIPreyBase.h"
#include "DataAssets/AI/AIPreyDataAsset.h"
#include "Managers/ArenaManager.h"
#include "Managers/CaptureGameManager.h"
#include "ActorComponents/HoldableComponent.h"
#include "Components/CapsuleComponent.h"
#include "ToolboxBPFL.h"

// Sets default values
AAIPreyBase::AAIPreyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HoldableComponent = CreateDefaultSubobject<UHoldableComponent>(TEXT("HoldableComponent"));
	if (HoldableComponent)
	{
		HoldableComponent->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void AAIPreyBase::BeginPlay()
{
	Super::BeginPlay();
	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();
	AArenaManager* ArenaManager = GameManager ? GameManager->GetArenaManager() : nullptr;
	if (ArenaManager)
	{
		ArenaManager->RegisterAIPrey(this);
	}
}


void AAIPreyBase::Captured()
{
	SetActorEnableCollision(false);
	if (HoldableComponent)
	{
		HoldableComponent->bIsEnabled = false;
		HoldableComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

int AAIPreyBase::GetScore()
{
	if (DataAsset)
	{
		return DataAsset->ScoreOnCapture;
	}
	return 0;
}

