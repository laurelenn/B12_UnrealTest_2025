// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIPreyDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UAIPreyDataAsset : public UDataAsset
{
	GENERATED_BODY()

	public : 

	/** Score to win when captured*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "0"))
	int ScoreOnCapture = 0;

	/** Distance at which player is detected - causes flight*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "1.0"))
	float DistancePlayerDetection = 100.f;

	/**Distance considered as safe after flight - will walk again*/
	UPROPERTY(Category = "General", EditAnywhere, meta = (ClampMin = "1.0"))
	float SafeDistance = 150.f;

	/**Normal speed when not on flight*/
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = "0.0"))
	float SpeedWalk = 0.f;

	/**Speed on flight - should be speeder that the SpeedWalk*/
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (ClampMin = "0.0"))
	float SpeedFlight = 0.f;

	// Can add a lot of others options following the gameplay features (mesh, size, material, type, if a can be escape prison....)

};
