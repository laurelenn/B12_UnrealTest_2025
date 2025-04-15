// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Projectile.generated.h"

class UProjectileLauncher;
class UShapeComponent;
class ULifeComponent;

UCLASS(Abstract)
class B12_TEST_LAURELENNSG_API AProjectile : public AActor
{
	GENERATED_BODY()
	

#pragma region ----- VARIABLES -----
protected :

/**Actor where we will try to apply damage if we hit them*/
UPROPERTY(Category = "#Setup", EditDefaultsOnly)
TArray<TSubclassOf<AActor>> DamageableActors;

/**Collision channels where the projectile will die if we hit it*/
UPROPERTY(Category = "#Setup", EditDefaultsOnly)
TArray<TEnumAsByte<ECollisionChannel>> SimpleImpactChannels;

/**Damages when a damageable actor is hit*/
UPROPERTY(Category = "#Setup", EditDefaultsOnly)
float Damages = 0;

/**Number of items he can hits before destruction*/
UPROPERTY(Category = "#Setup", EditDefaultsOnly, meta = (ClampMin = "1"))
int Life = 1;

/**For Optimization : The projectile will die if is out of this distance of the origin location*/
UPROPERTY(Category = "#Setup", EditDefaultsOnly)
float MaxReachableDistance = 10000.f;

/***/
UPROPERTY(Category = "Components", VisibleAnywhere)
ULifeComponent* LifeComponent;

UPROPERTY(Category = "Components", VisibleAnywhere)
UShapeComponent* Collision;


public : 
UPROPERTY(Category = Runtime, VisibleAnywhere)
AActor* OwnerActor;

UPROPERTY(Category = Runtime, VisibleAnywhere)
UProjectileLauncher* OwnerSocketComponent;

UPROPERTY(Category = Runtime, VisibleAnywhere)
FVector SpawnWorldLocation = FVector::ZeroVector;

UPROPERTY(Category = Runtime, VisibleAnywhere)
float CurrentDistanceFromSpawnPoint = 0;

protected : 
	UPROPERTY(Category = Runtime, VisibleAnywhere)
	TArray<AActor*> ActorsAlreadyHit;

#pragma endregion


#pragma region ----- VARIABLES -----

public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	virtual void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual bool IsDamageableActor(AActor* ActorHit);

	UFUNCTION(BlueprintCallable)
	virtual bool IsSimpleImpactChannel(ECollisionChannel Channel);

	UFUNCTION(BlueprintCallable)
	virtual void OnGameStateChange(ECaptureGameState State);

	UFUNCTION(BlueprintCallable)
	virtual void HandleGameStart(); // For pause / unpause

	UFUNCTION(BlueprintCallable)
	virtual void HandleGameEnd(); // For pause/end

	UFUNCTION(BlueprintCallable)
	virtual void OnLifeCompDeath();

	UFUNCTION(BlueprintCallable)
	virtual void DestroyProjectile();

	#pragma endregion

};
