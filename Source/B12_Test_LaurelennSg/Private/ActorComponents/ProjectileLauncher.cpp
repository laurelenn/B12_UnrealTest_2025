// Fill out your copyright notice in the Description page of Project Settings.
#include "ActorComponents/ProjectileLauncher.h"

// Sets default values for this component's properties
UProjectileLauncher::UProjectileLauncher()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}


// Called when the game starts
void UProjectileLauncher::BeginPlay()
{
	Super::BeginPlay();
	SetEnabled(bActivateAtStart);
}

void UProjectileLauncher::FirstLaunch()
{
	bFirstHitLaunched = true;
}

void UProjectileLauncher::PrepareLaunch()
{
}

void UProjectileLauncher::LaunchProjectile()
{
}

void UProjectileLauncher::SetEnabled(bool bIsEnabled)
{
	bEnabled = bIsEnabled;
}



