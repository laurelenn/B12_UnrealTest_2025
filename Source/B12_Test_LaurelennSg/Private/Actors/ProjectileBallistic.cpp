// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ProjectileBallistic.h"
#include "ActorComponents/LifeComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ToolboxBPFL.h"


AProjectileBallistic::AProjectileBallistic()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement_Component"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bAutoActivate = true;
}

void AProjectileBallistic::BeginPlay()
{
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->Velocity = Speed * GetActorForwardVector();
	ProjectileMovementComponent->Activate();
}

void AProjectileBallistic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentDistanceFromSpawnPoint > MaxReachableDistance)
	{
		DestroyProjectile();
	}
}



void AProjectileBallistic::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor != OwnerActor && !ActorsAlreadyHit.Contains(OtherActor)) // We don't want to multidamage the same actor with the same projectile
	{
		// Damageable Actors
		if (IsDamageableActor(OtherActor))
		{
			if (ULifeComponent* ActorLifeComp = UToolboxBPFL::GetLifeComponent(OtherActor))
			{
				ActorLifeComp->Damage(Damages, this);
				LifeComponent->Damage(1);
				ActorsAlreadyHit.AddUnique(OtherActor);
			}
		}
		else
		{
			// Simple Impact Actors
			if (IsSimpleImpactChannel(OverlappedComponent->GetCollisionObjectType()))
			{
				LifeComponent->Damage(1);
				ActorsAlreadyHit.AddUnique(OtherActor);
			}
		}
	}

}


void AProjectileBallistic::DestroyProjectile()
{
	SetActorEnableCollision(false);
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ZeroVector);
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	Super::DestroyProjectile();
}
