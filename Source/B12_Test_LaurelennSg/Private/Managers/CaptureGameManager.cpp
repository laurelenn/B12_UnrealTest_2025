// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CaptureGameManager.h"

#include "Managers/ArenaManager.h"
#include "Actors/CaptureZone.h"
#include "ToolboxBPFL.h"
#include "B12_Test_LaurelennSg/B12_Test_LaurelennSgCharacter.h"
#include "DataAssets/CaptureGameDataAsset.h"
#include "ActorComponents/LifeComponent.h"

#include "Widgets/GameCaptureWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"



void UCaptureGameManager::CustomTick()
{
    //Update manually the timer to display it without calling GetRemainingTime() on a biggest timer -- only if not in pause
    if (GameState == ECaptureGameState::Playing && DataAsset->bGameLimitedInTime)
    {
        RemainingGameTime -= DataAsset->TickDelay;
        if (IsValid(GameUI))
        {
            GameUI->UpdateTime(RemainingGameTime);
        }
        if (RemainingGameTime <= 0.f)
        {
            // The Timer is over !
            OnTimerEnds();
        }
    }
}

void UCaptureGameManager::SetGameState(ECaptureGameState NewState)
{
    PreviousGameState = GameState;
    GameState = NewState;


    switch (NewState)
    {
        case ECaptureGameState::Initialization :
        HandleInitialization();
        break;

        case ECaptureGameState::ReadyToStart :
        HandleReadyToStart();
        break;
        
        case ECaptureGameState::Playing :
        HandlePlaying();
        break; 
        
        case ECaptureGameState::Paused :
        HandlePaused();
        break;
         
        case ECaptureGameState::GameOver :
            bIsPlayerRegistered = false;
            bIsCaptureZoneRegistered = false;

            if (UWorld* World = GetWorld())
            {
                World->GetTimerManager().SetTimer(RestartTimerHandle, this, &UCaptureGameManager::UnloadMapLevels, 5.0f, false);
            }        break;

        case ECaptureGameState::ReadyToRestart:
            HandleReadyToRestart();
            break;

    }
    
    // Notify everyone the game state has changed
    OnGameStateChange.Broadcast(GameState);

}

void UCaptureGameManager::CheckIfReadyForStart()
{
    UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::CheckIfReadyForStart : Checking Ready For Start..."));

    bool bAllLevelsAreLoaded = (GDLevelsLoaded.Num() == DataAsset->GDLevelsToLoad.Num()) && ArtLevelsLoaded.Num() == DataAsset->ArtLevelsToLoad.Num();
	bool bArenaRegistered = IsValid(ArenaManager);
    bool bUiRegistered = IsValid(GameUI);
    bool bDataAssetRegistered = IsValid(DataAsset);

    if (bIsPlayerRegistered 
    && bIsCaptureZoneRegistered 
    && bArenaRegistered
    && bUiRegistered
    && bDataAssetRegistered
    && bAllLevelsAreLoaded)
	{
		SetGameState(ECaptureGameState::ReadyToStart);
	}
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UCaptureGameManager::CheckIfReadyForStart : Not initialized for the moment: - bIsPlayerRegistered: %d - bIsCaptureZoneRegistered: %d - bArenaRegistered: %d - bUiRegistered: %d - bDataAssetRegistered: %d - bAllLevelsAreLoaded: %d"),
            bIsPlayerRegistered,
            bIsCaptureZoneRegistered,
            bArenaRegistered,
            bUiRegistered,
            bDataAssetRegistered,
            bAllLevelsAreLoaded);
    }
}

void UCaptureGameManager::LoadMapLevels()
{
    UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::LoadMapLevels : Launch Level Streaming..."));

    UWorld* World = GetWorld();

    // Load GameDesign levels
    for (const TSoftObjectPtr<UWorld>& LevelAsset : DataAsset->GDLevelsToLoad)
    {
       FString  LevelName = *LevelAsset.ToSoftObjectPath().GetLongPackageName();

        bool IsSuccess;
        ULevelStreaming* LevelLoaded = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), LevelAsset, FVector::ZeroVector, FRotator::ZeroRotator, IsSuccess);

        if (IsSuccess)
        {
            GDLevelsLoaded.Add(LevelAsset.Get());
            UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::LoadMapLevels : %s GD Map Loaded!"), *LevelName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::LoadMapLevels : Failed to load GD Map: %s"), *LevelName);
        }
    }

    // Load Art levels
    for (const TSoftObjectPtr<UWorld>& LevelAsset : DataAsset->ArtLevelsToLoad)
    {
        FString  LevelName = *LevelAsset.ToSoftObjectPath().GetLongPackageName();

        bool IsSuccess;
        ULevelStreaming* LevelLoaded = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), LevelAsset, FVector::ZeroVector, FRotator::ZeroRotator, IsSuccess);

        if (IsSuccess)
        {
            ArtLevelsLoaded.Add(LevelAsset.Get());
            UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::LoadMapLevels : %s Art Map Loaded!"), *LevelName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::LoadMapLevels : Failed to load Art Map: %s"), *LevelName);
        }
    }

    CheckIfReadyForStart();
}

void UCaptureGameManager::UnloadMapLevels()
{
    // Clean all GD levels
    for (UWorld* Level : GDLevelsLoaded)
    {
        if (Level)
        {
            FName LevelName = Level->GetFName();
            UGameplayStatics::UnloadStreamLevel(this, LevelName, {}, false); // NOTE : Should add a delegate to count if all levels has been unload before go to the next part
        }
    }

    GDLevelsLoaded.Empty();

    // Reload All GD levels
    for (TSoftObjectPtr<UWorld> LevelToLoad : DataAsset->GDLevelsToLoad)
    {
        if (LevelToLoad.IsValid())
        {
            FString  LevelName = *LevelToLoad.ToSoftObjectPath().GetLongPackageName();

            bool IsSuccess;
            ULevelStreaming* LevelLoaded = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), LevelToLoad, FVector::ZeroVector, FRotator::ZeroRotator, IsSuccess);

            if (IsSuccess)
            {
                GDLevelsLoaded.Add(LevelToLoad.Get());
            }
        }
    }
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(UnloadMapsTimerHandle, this, &UCaptureGameManager::LaunchReadyToRestart, 4.0f, false);
    }
}

void UCaptureGameManager::InitGameManager(UCaptureGameDataAsset* DA)
{
    UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::InitGameManager : Launch Initialization..."));

    DataAsset = DA;
    if (DataAsset && DataAsset->WidgetClassToSpawn)
    {
        LoadMapLevels();
        GameUI = CreateWidget<UGameCaptureWidget>(GetWorld(), DataAsset->WidgetClassToSpawn);

        //Create Game UI and Init it
        if (IsValid(GameUI))
        {
            GameUI->AddToViewport();

            GameUI->SetVisibility(ESlateVisibility::Visible);

            UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;

            GameUI->InitWidget(this);
            UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::InitGameManager : Widget created successfully !"));
            CheckIfReadyForStart(); // Check if everything is initialized
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::InitGameManager : Widget was not created !"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::InitGameManager : Invalid widget class provided or DataAsset !"));
    }
}

bool UCaptureGameManager::RegisterPlayer(AB12_Test_LaurelennSgCharacter* Player)
{
    if (IsValid(Player))
    {
        // Register player to be bind to his death event (end of game condition)
        ULifeComponent* playerLifeComp = UToolboxBPFL::GetLifeComponent(Player);
        if (IsValid(playerLifeComp))
        {
            playerLifeComp->OnDeath.AddDynamic(this, &UCaptureGameManager::OnPlayerDied);
            bIsPlayerRegistered = true;
            UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::RegisterPlayer : Player registered successfully!"));

            CheckIfReadyForStart(); // Check if everything is init
            return true;
        }
        UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::RegisterPlayer : Invalid player's life comp !"));
        return false;
    }
    UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::RegisterPlayer : Invalid player provided !"));
	return false;
}

bool UCaptureGameManager::RegisterArenaManager(AArenaManager* Arena)
{
    // Register arena to be bind on his end event (end of game condition)
    if (IsValid(Arena))
    {
        ArenaManager = Arena;
        Arena->OnArenaEnd.AddDynamic(this, &UCaptureGameManager::OnArenaEnds);
        UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::RegisterArena : Arena registered successfully!"));

        CheckIfReadyForStart(); // Check if everything is init
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::RegisterArena : Invalid Arena provided !"));
    return false;
}

bool UCaptureGameManager::RegisterCaptureZone(ACaptureZone* CaptureZone)
{
    if (IsValid(CaptureZone))
    {
        CaptureZone->OnPreyCaptured.AddDynamic(this, &UCaptureGameManager::OnPreyCaptured);
        bIsCaptureZoneRegistered = true;
        UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::RegisterCaptureZone : CaptureZone registered successfully!"));

        CheckIfReadyForStart(); // Check if everything is init
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::RegisterCaptureZone : Invalid CaptureZone provided !"));
    return false;
}

void UCaptureGameManager::OnPlayerDied(ULifeComponent* LifeComponent, AActor* Causer)
{
    UE_LOG(LogTemp, Log, TEXT("UCaptureGameManager::OnPlayerDied : Did you just died ?"));
    LaunchGameOver(ECaptureGameOverReason::PlayerDeath);
}

void UCaptureGameManager::OnArenaEnds()
{
    if (DataAsset && DataAsset->bCanEndWithArena)
    {
       LaunchGameOver(ECaptureGameOverReason::ArenaWavesOver);
    }
}

void UCaptureGameManager::OnPreyCaptured(int ScorePrey)
{
    Score +=ScorePrey;
    if (IsValid(GameUI))
    {
        GameUI->UpdateScore(Score);
    }
}

void UCaptureGameManager::OnTimerEnds()
{
    LaunchGameOver(ECaptureGameOverReason::TimeOver);
}


void UCaptureGameManager::LaunchGameOver(ECaptureGameOverReason Reason)
{

    // Unbind from all the the registered event
    // Reset all the init conditions

    SetGameState(ECaptureGameState::GameOver);
    if (IsValid(GameUI))
    {
        GameUI->DisplayGameOverPanel(Reason);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UCaptureGameManager::LaunchGameOver : Invalid GameUI, can't update the UI !"));
    }
}

void UCaptureGameManager::HandleInitialization()
{
    LoadMapLevels();
}

void UCaptureGameManager::HandleReadyToStart()
{
    // Clean the timer if we need to use it
    if (DataAsset->bGameLimitedInTime)
    {
        if (GetWorld() && TickTimerHandle.IsValid())
        {
            GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
        }
    }

    RemainingGameTime = DataAsset->GameDuration;
    Score = 0;
}

void UCaptureGameManager::HandlePlaying()
{
    // Start the timer only if we need to
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ThisClass::CustomTick, DataAsset->TickDelay, true);
    }
}

void UCaptureGameManager::HandlePaused()
{
//  stop the timer 
//  the broadcast of this state will notify all the IA to inactive them
//  unactive the "holdable" capacity of the player

    if (GetWorld() && TickTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
    }
}

void UCaptureGameManager::HandleUnpaused()
{
    // Return to the previous state (playing, ready for restart etc...)  
    SetGameState(PreviousGameState);
}


void UCaptureGameManager::HandleReadyToRestart()
{
// Nothing here for the moment

}


// Game over 
// NOTE : All features in this part are not usefull for the moment cause we use a lot of dynamic events, 
// but it could be for some specific features (VFX & SFX call, call on specific actors in scene that didn't need to be bind to all the cases of events...)
void UCaptureGameManager::HandleGameOverPreysCaptured()
{
}

void UCaptureGameManager::HandleGameOverTimeout()
{
}

void UCaptureGameManager::HandleGameOverArenaEnd()
{
}

void UCaptureGameManager::HandleGameOverPlayerDeath()
{
}
