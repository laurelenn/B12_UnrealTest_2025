// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "ArenaManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArenaEnd);

class AAIPreyBase;
class UBoxComponent;

USTRUCT(Blueprintable)
struct FWave
{
	GENERATED_BODY()

	/**The layers to load during a wave. Usefull if you want specific location for the actors to spawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LayerWave"))
	TArray<TSoftObjectPtr<UWorld>> LayersToLoad;

	/**The layers to load during a wave. Usefull if you want random locations for the actors to spawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Actors To Spawn"))
	TMap<TSubclassOf<AActor>, int> ActorsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "End Wave Condition", EditCondition = "!bIsLastWave", EditConditionHides)) // Edit Conditoin ? 
	EArenaWaveEndCondition EndWaveCondition = EArenaWaveEndCondition::TimerOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", DisplayName = "DelayNextWave", EditCondition = "!bIsLastWave", EditConditionHides))
	float DelayNextWave = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsLastWave = false;
};


UCLASS()
class B12_TEST_LAURELENNSG_API AArenaManager : public AActor
{
	GENERATED_BODY()
	
	#pragma region ----- VARIABLES -----
public : 

	FOnArenaEnd OnArenaEnd;

private : 

#pragma region Setup

	//// Setup GD

	/**Condition to considered arena finished*/
	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "End Condition"))
	EArenaEndCondition EndCondition = EArenaEndCondition::TimerOnly;

	/**Delay to notify everyone the Arena is finished after the end condition is valid*/
	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", DisplayName = "Delay After End Arena Condition"))
	float DelayAfterEndCondition = 0.0f;

	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "Waves"))
	TArray<FWave> Waves;

	/**Elements to ignore on the wave end conditions and arena end conditions*/
	UPROPERTY(Category = "#Setup|AdvancedDev", BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayName = "IgnoredElements"))
	TArray<TSubclassOf<AActor>> IgnoredElements;

	#pragma endregion

#pragma region Runtime and EndConditions

	bool bEndLaunched = false;

	//// Loaded Elements 
	UPROPERTY(Category = Runtime, VisibleAnywhere, meta = (DisplayName = "Layers Loaded"))
	TArray<ULevelStreaming*> LayersLoaded;

	UPROPERTY(Category = Runtime, VisibleAnywhere, meta = (DisplayName = "AI Prey Loaded"))
	TArray<AAIPreyBase*> AIPreysLoaded;

	//// Counter total Elements
	UPROPERTY(Category = Runtime, VisibleAnywhere, meta = (DisplayName = "CurrentWave"))
	int CurrentWave = -1;

	UPROPERTY(Category = Runtime, VisibleAnywhere, meta = (DisplayName = "Number Preys Left Arena"))
	int AIPreysLeftArena = 0;

	//// Couter wave Elements
	UPROPERTY(Category = Runtime, VisibleAnywhere, meta = (DisplayName = "Number Preys Left Wave"))
	TArray<AAIPreyBase*> AIPreysLeftWave;


	// Timers
	FTimerHandle TimerNextWave;
	FTimerHandle TimerEnd;
	FTimerHandle TimerLastWaveCheckCondition;
	FTimerHandle TimerWaveCheckCondition;

#pragma endregion


#pragma region ----- FUNCTIONS -----

public:	
	// Sets default values for this actor's properties
	AArenaManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private :	
	
#pragma region Game State Management
	UFUNCTION(BlueprintCallable)
	void HandleGameStart();

	UFUNCTION(BlueprintCallable)
	void HandleGameEnd();

	UFUNCTION(BlueprintCallable)
	void OnGameStateChange(ECaptureGameState State);
	#pragma endregion

	#pragma region Wave Management

	UFUNCTION()
	void ResetWaveCounters();

	UFUNCTION(CallInEditor)
	void LaunchNextWave();

	UFUNCTION()
	void LoadLevelsWave(int CurrentWaveIndex);

	UFUNCTION()
	void LoadActorsWave(int CurrentWaveIndex);

#pragma region Registration

// Conditions Elements will auto-register them on begin play

public : 

	UFUNCTION()
	void RegisterAIPrey(AAIPreyBase* AICaptured);
#pragma endregion

private :

	UFUNCTION()
	void AIPreyCaptured(AAIPreyBase* AICaptured);

	UFUNCTION()
	bool IsLastWave();

	UFUNCTION()
	void VerifEndWaveCondition();

	UFUNCTION()
	void LaunchTimerNextWave();
#pragma endregion





#pragma region Arena End Management

	UFUNCTION()
	void VerifEndArenaCondition();

	UFUNCTION()
	void LaunchTimerEndArena();


	UFUNCTION()
	void EndArena();
#pragma endregion



	//// EDITOR

#if WITH_EDITOR

	void EditLastWave();

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif

#pragma endregion
};
