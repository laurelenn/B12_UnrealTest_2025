// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBase.h"
#include "AIEnemyBase.generated.h"

UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API AAIEnemyBase : public AAIBase
{
	GENERATED_BODY()

#pragma region ----- VARIABLES -----

#pragma endregion

#pragma region ----- FUNCTIONS -----


public:
	// Sets default values for this character's properties
	AAIEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	#pragma endregion
};
