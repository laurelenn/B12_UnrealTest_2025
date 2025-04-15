// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/AIBase.h"
#include "Managers/CaptureGameManager.h"
#include "ToolboxBPFL.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AAIBase::AAIBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	LifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("LifeComponent"));
}

void AAIBase::BeginPlay()
{
	Super::BeginPlay();
	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();

	if (GameManager)
	{
		GameManager->OnGameStateChange.AddDynamic(this, &AAIBase::OnGameStateChange);
	}

}

void AAIBase::OnGameStateChange(ECaptureGameState GameState)
{
	if (GameState != ECaptureGameState::Playing)
	{
		SetIsActive(false);
	}

}

