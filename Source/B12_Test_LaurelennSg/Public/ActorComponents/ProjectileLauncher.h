// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ProjectileLauncher.generated.h"

class UNiagaraSystem;

UCLASS(Abstract)
class B12_TEST_LAURELENNSG_API UProjectileLauncher : public USceneComponent
{
	GENERATED_BODY()

#pragma region ----- VARIABLES -----
public:

	/**If true, will start launch projectile at begin play*/
	UPROPERTY(Category = "#Setup", EditAnywhere)
	bool bActivateAtStart = true;

	/**Will spawn on fire*/
	UPROPERTY(Category = "#Setup", EditAnywhere)
	UNiagaraSystem* VFXOnLaunch = nullptr;

	/**Delay before first Launch*/
	UPROPERTY(Category = "#Setup", EditAnywhere, meta = (ClampMin = "0.0"))
	float DelayFirstLaunch = 0.f;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	bool bFirstHitLaunched = false;

	FTimerHandle TimerFirstLaunch;
#pragma endregion


#pragma region ----- FUNCTIONS -----


public:
	// Sets default values for this component's properties
	UProjectileLauncher();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void FirstLaunch();

	UFUNCTION(BlueprintCallable)
	virtual void PrepareLaunch();

	UFUNCTION(BlueprintCallable, CallInEditor)
	virtual void LaunchProjectile();

#pragma endregion

};
