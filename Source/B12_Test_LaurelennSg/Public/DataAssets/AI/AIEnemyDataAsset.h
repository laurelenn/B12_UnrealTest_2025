// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIEnemyDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UAIEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()

	public : 
	/**Speed to follow player when he is detected*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "0.5"))
	float Speed = 300.f;

	/**Max Distance to detect and follow-shoot on player - Can't follow/hit the player if he's too far*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "0.5"))
	float MaxDistancePlayerDetect = 400.f;

	/**The distance the AI will try to keep*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "0.5"))
	float DistanceHuntPlayer = 200.f;
};
