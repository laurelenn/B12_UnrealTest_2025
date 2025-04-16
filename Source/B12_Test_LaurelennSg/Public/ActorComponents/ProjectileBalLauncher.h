// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ProjectileLauncher.h"
#include "ProjectileBalLauncher.generated.h"

class AProjectileBallistic;
class UNiagaraSystem;


UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UProjectileBalLauncher : public UProjectileLauncher
{
	GENERATED_BODY()

#pragma region ----- VARIABLES -----
	public : 

	/**Projectile class to spawn*/
	UPROPERTY(Category = "#Setup", EditAnywhere)
	TSubclassOf<AProjectileBallistic> ProjectileClass;

	/**All hits will spawn by "round" of "nbHits projectiles" : Delay between rounds*/
	UPROPERTY(Category = "#Setup", EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.01f, Units = "Seconds"))
	float DelayBetweenRounds = 1.0f;

	/**All hits will spawn by "round" of "nbHits projectiles" : Nb hits in one round*/
	UPROPERTY(Category = "#Setup", EditDefaultsOnly, BlueprintReadOnly, meta = (Units = "Times"))
	int NbHits = 1;

	/**All hits will spawn by "round" of "nbHits projectiles" : Delay between hits (in round)*/
	UPROPERTY(Category = "#Setup", EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.01f, Units = "Seconds"))
	float DelayBetweenHits = 0.01f;

	protected : 

	/**All hits will spawn by "round" of "nbHits projectiles" : Nb hits launched in the current round*/
	UPROPERTY(Category = Runtime, VisibleAnywhere)
	int NbHitsLaunched = 0;

	UPROPERTY(Category = Runtime, VisibleAnywhere)
	bool bEnabled = false;

	FTimerHandle TimerNextLaunch;
#pragma endregion


#pragma region ----- FUNCTIONS -----


public:	
	// Sets default values for this component's properties
	UProjectileBalLauncher();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void FirstLaunch() override;

	virtual void PrepareLaunch() override;

	virtual void LaunchProjectile() override;

public  :

	UFUNCTION(Category = "API")
	void SetEnabled(bool bIsEnabled);
#pragma endregion

};
