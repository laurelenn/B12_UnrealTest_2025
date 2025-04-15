// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ProjectileBallistic.h"
#include "ActorComponents/LifeComponent.h"
#include "ToolboxBPFL.h"

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
