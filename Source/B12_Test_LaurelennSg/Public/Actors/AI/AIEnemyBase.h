// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBase.h"
#include "AIEnemyBase.generated.h"

class UProjectileBalLauncher;
class UAIEnemyDataAsset;

UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API AAIEnemyBase : public AAIBase
{
	GENERATED_BODY()

#pragma region ----- VARIABLES -----

protected:

	UPROPERTY(Category = "Components", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileBalLauncher* ProjectileLauncherComp;

public:

	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAIEnemyDataAsset* DataAsset;

#pragma endregion

#pragma region ----- FUNCTIONS -----


public:
	// Sets default values for this character's properties
	AAIEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(Category = "API", BlueprintCallable)
	UProjectileBalLauncher* GetProjectileLauncherComp() { return ProjectileLauncherComp; }

	#pragma endregion
};
