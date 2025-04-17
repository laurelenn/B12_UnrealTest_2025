// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ProjectileBalLauncher.h"
#include "Actors/ProjectileBallistic.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UProjectileBalLauncher::UProjectileBalLauncher()
{
}


// Called when the game starts
void UProjectileBalLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectileBalLauncher::FirstLaunch()
{
	Super::FirstLaunch();
	UE_LOG(LogTemp, Log, TEXT("UProjectileBalLauncher::FirstLaunch : First Launch fired !"));

	PrepareLaunch();

}


void UProjectileBalLauncher::PrepareLaunch()
{
	// One hit on one round
	LaunchProjectile();
	NbHitsLaunched++;


	// Check if round finished
	if (NbHitsLaunched >= NbHits)
	{
		UE_LOG(LogTemp, Log, TEXT("UProjectileBalLauncher::PrepareLaunch : Round finished, prepare next"));

		// Round finished
		GetWorld()->GetTimerManager().ClearTimer(TimerNextLaunch);
		GetWorld()->GetTimerManager().SetTimer(TimerNextLaunch, this, &UProjectileBalLauncher::PrepareLaunch, DelayBetweenRounds, false);

		NbHitsLaunched = 0;
	}
	else
	{ // Same round -- prepare next hit
		GetWorld()->GetTimerManager().ClearTimer(TimerNextLaunch);
		GetWorld()->GetTimerManager().SetTimer(TimerNextLaunch, this, &UProjectileBalLauncher::PrepareLaunch, DelayBetweenHits, false);


	}
}

void UProjectileBalLauncher::LaunchProjectile()
{
	if (UKismetSystemLibrary::IsValidClass(ProjectileClass))
	{
		if (GetWorld())
		{
			FTransform SpawnTransform = FTransform(GetComponentTransform().GetRotation(), GetComponentTransform().GetLocation(), FVector::OneVector);
			FActorSpawnParameters SpawnInfo;

			AProjectileBallistic* ProjectileSpawned = GetWorld()->SpawnActorDeferred<AProjectileBallistic>(ProjectileClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			if (ProjectileSpawned)
			{

			ProjectileSpawned->OwnerActor = GetOwner();
			ProjectileSpawned->OwnerSocketComponent = this;

			ProjectileSpawned->FinishSpawning(SpawnTransform);

				//VFX
				if (VFXOnLaunch)
				{
					UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXOnLaunch, GetComponentLocation());
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UProjectileBalLauncher::LaunchProjectile :Projectile not spawned !"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UProjectileBalLauncher::LaunchProjectile : Invalid world provided"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UProjectileBalLauncher::LaunchProjectile : Invalid projectile class provided"));
	}
}

void UProjectileBalLauncher::SetEnabled(bool bIsEnabled)
{
	Super::SetEnabled(bIsEnabled);
	if (bIsEnabled)
	{
		UE_LOG(LogTemp, Log, TEXT("UProjectileBalLauncher::SetEnabled : Projectile Launcher Enabled !"));

		if (!bFirstHitLaunched)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerFirstLaunch);
			GetWorld()->GetTimerManager().SetTimer(TimerFirstLaunch, this, &UProjectileBalLauncher::FirstLaunch, DelayFirstLaunch, false);
		}
		else
		{
			PrepareLaunch();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UProjectileBalLauncher::SetEnabled : Projectile Launcher Disabled !"));

		GetWorld()->GetTimerManager().ClearTimer(TimerFirstLaunch);
		GetWorld()->GetTimerManager().ClearTimer(TimerNextLaunch);
	}
}

