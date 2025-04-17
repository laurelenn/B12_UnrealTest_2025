// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AI/AIEnemyBase.h"
#include "ActorComponents/ProjectileBalLauncher.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ToolboxBPFL.h"

// Sets default values
AAIEnemyBase::AAIEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ProjectileLauncherComp = CreateDefaultSubobject<UProjectileBalLauncher>(TEXT("ProjectileLauncherComp"));
	if (ProjectileLauncherComp)
	{
		ProjectileLauncherComp->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void AAIEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

