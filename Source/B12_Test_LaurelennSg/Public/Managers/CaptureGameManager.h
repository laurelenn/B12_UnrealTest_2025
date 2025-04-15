// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Enums.h"
#include "CaptureGameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChange, ECaptureGameState, GameState);



class AB12_Test_LaurelennSgCharacter;
class AArenaManager;
class ACaptureZone;
class UGameCaptureWidget;
class UCaptureGameDataAsset;

UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UCaptureGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
#pragma region ----- VARIABLES -----

	//Only if it's an actor
	///** State to switch on for DEBUG */
	//UPROPERTY(Category = "#Debug", EditAnywhere)
	//ECaptureGameState DebugGameState = ECaptureGameState::Initialization;


protected : 


	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	UCaptureGameDataAsset* DataAsset;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	ECaptureGameState GameState = ECaptureGameState::Initialization;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	ECaptureGameState PreviousGameState = ECaptureGameState::Initialization;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	int Score = 0;

	/** Timer to fake a tick ! */
	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	FTimerHandle TickTimerHandle;
	
	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	FTimerHandle RestartTimerHandle;
	
	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	FTimerHandle UnloadMapsTimerHandle;

	// Note : I use instead a timerHandle to avoid calling "GetRemainingTime()" to display it
	/** Time remaining before the end of the Timer (useful if bGameLimitedInTime = true) */
	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	float RemainingGameTime = 0.f;

	// Everything here check if the things to register are initialized/register or not, if not, we can't play the game
	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	bool bIsPlayerRegistered = false;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	bool bIsCaptureZoneRegistered = false;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	AArenaManager* ArenaManager;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	UGameCaptureWidget* GameUI;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	TArray<UWorld*> GDLevelsLoaded;

	UPROPERTY(Category = Runtime, BlueprintReadOnly)
	TArray<UWorld*> ArtLevelsLoaded;

	/// EVENTS
	public : 

	UPROPERTY(BlueprintCallable);
	FOnGameStateChange OnGameStateChange;
#pragma endregion


#pragma region ----- FUNCTIONS -----

protected:
	// Called every "frame" (fake tick)
	UFUNCTION()
	void CustomTick();

public : 

#pragma region Getters

UFUNCTION(Category = "API", BlueprintCallable)
ECaptureGameState GetGameState() {return GameState;}

UFUNCTION(Category = "API", BlueprintCallable)
float GetRemainingGameTime() {return RemainingGameTime;}

UFUNCTION(Category = "API", BlueprintCallable)
int GetScore() {return Score;}

UFUNCTION(Category = "API", BlueprintCallable)
AArenaManager* GetArenaManager(){return ArenaManager;}

#pragma endregion

#pragma region Setters

UFUNCTION(Category = "API", BlueprintCallable)
void SetGameState(ECaptureGameState NewState);

#pragma endregion

protected:

#pragma region Init & Registration

/** Check if initialization is handed and switch gameState */
UFUNCTION(BlueprintCallable)
void CheckIfReadyForStart();

void LoadMapLevels();
void UnloadMapLevels();

public : 

UFUNCTION(BlueprintCallable)
void InitGameManager(UCaptureGameDataAsset* DA);

UFUNCTION(BlueprintCallable)
bool RegisterPlayer(AB12_Test_LaurelennSgCharacter* Player);

UFUNCTION(BlueprintCallable)
bool RegisterArenaManager(AArenaManager* Arena);

UFUNCTION(BlueprintCallable)
bool RegisterCaptureZone(ACaptureZone* CaptureZone);

#pragma endregion

private:

#pragma region Handle Gameplay Events

UFUNCTION(BlueprintCallable)
void OnPlayerDied(ULifeComponent* LifeComponent, AActor* Causer);

UFUNCTION(BlueprintCallable)
void OnArenaEnds();

UFUNCTION(BlueprintCallable)
void OnPreyCaptured(int ScorePrey);

UFUNCTION(BlueprintCallable)
void OnTimerEnds();

#pragma endregion


#pragma region GameStates Handle

UFUNCTION(Category = "GameStates", BlueprintCallable)
void LaunchGameOver(ECaptureGameOverReason Reason);

UFUNCTION(Category = "GameStates", BlueprintCallable)
void HandleInitialization();

UFUNCTION(Category = "GameStates", BlueprintCallable)
void HandleReadyToStart();

UFUNCTION(Category = "GameStates", BlueprintCallable)
void HandlePlaying();

UFUNCTION(Category = "GameStates", BlueprintCallable)
void HandlePaused();

UFUNCTION(Category = "GameStates", BlueprintCallable)
void HandleUnpaused();

UFUNCTION(Category = "GameStates", BlueprintCallable)
void LaunchReadyToRestart(){SetGameState(ECaptureGameState::ReadyToRestart);}

UFUNCTION(Category = "GameStates", BlueprintCallable)
void HandleReadyToRestart();

#pragma endregion

#pragma region GameOver Handle


UFUNCTION(Category = "Game Over", BlueprintCallable)
void HandleGameOverPreysCaptured();

UFUNCTION(Category = "Game Over", BlueprintCallable)
void HandleGameOverTimeout();

UFUNCTION(Category = "Game Over", BlueprintCallable)
void HandleGameOverArenaEnd();

UFUNCTION(Category = "Game Over", BlueprintCallable)
void HandleGameOverPlayerDeath();

#pragma endregion


#pragma region Debug
// Only if it's an actor
//UFUNCTION(Category = "#Debug", CallInEditor)
//void SwitchGameStateDEBUG() {SetGameState(DebugGameState); }

#pragma endregion

#pragma endregion

};
