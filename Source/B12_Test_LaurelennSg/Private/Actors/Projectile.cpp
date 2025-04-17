// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile.h"
#include "Components/ShapeComponent.h"
#include "Managers/CaptureGameManager.h"
#include "ActorComponents/LifeComponent.h"
#include "ToolboxBPFL.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("LifeComponent"));
	if (LifeComponent)
	{
		LifeComponent->InitStartParams(Life, 0, Life);
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	SpawnWorldLocation = GetActorLocation();

	Super::BeginPlay();
	if (Collision)
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHitActor);
	}

	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();
	if (GameManager)
	{
		GameManager->OnGameStateChange.AddDynamic(this, &AProjectile::OnGameStateChange);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AProjectile::BeginPlay : No Game Manager found !!"));
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentDistanceFromSpawnPoint = FVector::Dist(SpawnWorldLocation, GetActorLocation());
}


void AProjectile::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Override in children 
}

bool AProjectile::IsDamageableActor(AActor* ActorHit)
{
	if (ActorHit)
	{
		if (UToolboxBPFL::ContainsChildOf(DamageableActors,ActorHit->GetClass()))
		{
			return true;
		}
	}
	return false;
}

bool AProjectile::IsSimpleImpactChannel(ECollisionChannel Channel)
{
	return SimpleImpactChannels.Contains(Channel);
}

void AProjectile::OnGameStateChange(ECaptureGameState State)
{
	if (State == ECaptureGameState::Playing)
	{
		HandleGameStart();
	}
	else if (State == ECaptureGameState::GameOver)
	{
		HandleGameEnd();
	}
}

void AProjectile::HandleGameStart()
{
	if (Collision)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AProjectile::HandleGameEnd()
{
	if (Collision)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AProjectile::OnLifeCompDeath()
{
	DestroyProjectile();
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}


