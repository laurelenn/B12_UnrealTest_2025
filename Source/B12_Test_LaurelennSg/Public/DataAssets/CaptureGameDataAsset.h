// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CaptureGameDataAsset.generated.h"

/**
 * 
 */
class UGameCaptureWidget;


UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UCaptureGameDataAsset : public UDataAsset
{
	GENERATED_BODY()

	public : 

	/** Check if you want the game to end when the arena is finished */
	UPROPERTY(Category = "General", EditAnywhere)
	bool bCanEndWithArena = false; // NOTE : Replace by enum bitflags


	/** Check if you want the game to be time-limited */
	UPROPERTY(Category = "Timer", EditAnywhere)
	bool bGameLimitedInTime = true; // NOTE : Replace by enum bitflags

	/** Duration of the game before game over
	* This property is only visible when bGameLimitedInTime is true */
	UPROPERTY(Category = "Timer", EditAnywhere, meta = (ClampMin = "0.0", EditCondition = "bGameLimitedInTime", EditConditionHides, Units = seconds))
	float GameDuration = 0.f;



	/**Really usefull to test differents Art maps with the same gamedesign map and vice versa*/
	UPROPERTY(Category = "Levels", EditAnywhere)
	TArray<TSoftObjectPtr<UWorld>> GDLevelsToLoad;

	/**Really usefull to test differents Art maps with the same gamedesign map and vice versa*/
	UPROPERTY(Category = "Levels", EditAnywhere)
	TArray<TSoftObjectPtr<UWorld>> ArtLevelsToLoad;

	/** Delay used for fakeTick - Be careful for performances
	* This property is only visible when bGameLimitedInTime is true */
	UPROPERTY(Category = "Advanced Dev", EditAnywhere, meta = (ClampMin = "0.1", EditCondition = "bGameLimitedInTime", EditConditionHides))
	float TickDelay = 1.f;

	/** Widget class to make spawn for the game*/
	UPROPERTY(Category = "Advanced Dev", EditAnywhere)
	TSubclassOf<UGameCaptureWidget> WidgetClassToSpawn;

};
