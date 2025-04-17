// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile.h"
#include "ProjectileBallistic.generated.h"

/**
 * 
 */

 class UProjectileMovementComponent;
 class USphereComponent;

UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API AProjectileBallistic : public AProjectile
{
	GENERATED_BODY()

#pragma region ----- VARIABLES -----

	UPROPERTY(Category = "Components", VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(Category = "Components", EditDefaultsOnly)
	USphereComponent* SphereComponent;

#pragma endregion


#pragma region ----- FUNCTIONS -----
	AProjectileBallistic();
	void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	virtual void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void DestroyProjectile() override;
#pragma endregion

	
};
