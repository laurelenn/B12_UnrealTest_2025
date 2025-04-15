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
	float Speed = 10.f;

	/**Max Distance to detect and follow-shoot on player - Can't follow/hit the player if he's too far*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "0.5"))
	float MaxDistancePlayerDetect = 100.f;
	
	/**Min Distance from the player - Will move further if the player is too close*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "0.5"))
	float MinDistancePlayer = 10.f;
};
