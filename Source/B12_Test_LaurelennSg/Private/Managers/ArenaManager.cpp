// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ArenaManager.h"
#include "Components/BoxComponent.h"
#include "Managers/CaptureGameManager.h"
#include "Actors/AI/AIPreyBase.h"
#include "ToolboxBPFL.h"
#include <Engine/LevelStreamingDynamic.h>
#include <NavigationSystem.h>



// Sets default values
AArenaManager::AArenaManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AArenaManager::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("AArenaManager::BeginPlay : Arena Manager Begin Play..."));
	Super::BeginPlay();
	
	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();
	if (GameManager)
	{
		GameManager->OnGameStateChange.AddDynamic(this, &AArenaManager::OnGameStateChange);
		GameManager->RegisterArenaManager(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AArenaManager::BeginPlay : No Game Manager found !!"));
	}
}

void AArenaManager::HandleGameStart()
{
	LaunchNextWave();
}

void AArenaManager::HandleGameEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerNextWave);
	GetWorld()->GetTimerManager().ClearTimer(TimerEnd);
	GetWorld()->GetTimerManager().ClearTimer(TimerLastWaveCheckCondition);
	GetWorld()->GetTimerManager().ClearTimer(TimerWaveCheckCondition);
}

void AArenaManager::OnGameStateChange(ECaptureGameState State)
{
	if (State == ECaptureGameState::Playing)
	{
		HandleGameStart();
	}
	else if (State == ECaptureGameState::GameOver)
	{
		HandleGameEnd();
	}
}

void AArenaManager::ResetWaveCounters()
{
	AIPreysLeftWave.Empty();
}

void AArenaManager::LaunchNextWave()
{

	ResetWaveCounters();
	CurrentWave++;
	UE_LOG(LogTemp, Log, TEXT("AArenaManager::LaunchNextWave : Launch Next Wave : %d"), CurrentWave);
	bool IsLevelLoad = false;

	if (Waves.IsValidIndex(CurrentWave) && Waves.Num() > 0)
	{
		LoadLevelsWave(CurrentWave);
		LoadActorsWave(CurrentWave);

		if (Waves[CurrentWave].EndWaveCondition == EArenaWaveEndCondition::TimerOnly)
		{
			LaunchTimerNextWave();
		}
		
		if (IsLastWave())
		{
			GetWorldTimerManager().ClearTimer(TimerLastWaveCheckCondition);
			GetWorldTimerManager().SetTimer(TimerLastWaveCheckCondition, this, &AArenaManager::VerifEndArenaCondition, 1.f, false); // Wait a bit before checking if last condition is done
		}
		else
		{

			GetWorldTimerManager().ClearTimer(TimerWaveCheckCondition);
			GetWorldTimerManager().SetTimer(TimerWaveCheckCondition, this, &AArenaManager::VerifEndWaveCondition, 1.f, false); // Wait a bit before checking if last condition is done
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerLastWaveCheckCondition);
		GetWorldTimerManager().SetTimer(TimerLastWaveCheckCondition, this, &AArenaManager::VerifEndArenaCondition, 1.f, false); // Wait a bit before checking if last condition is done
	}

}

void AArenaManager::LoadLevelsWave(int CurrentWaveIndex)
{
	FWave currentWaveStruct = Waves[CurrentWaveIndex];

	for (const TSoftObjectPtr<UWorld>& Layer : currentWaveStruct.LayersToLoad)
	{
		bool IsLevelLoaded;
		ULevelStreaming* LevelLoaded = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), Layer, FVector::ZeroVector, FRotator::ZeroRotator, IsLevelLoaded);
		
		if (IsLevelLoaded)
		{
			LayersLoaded.Add(LevelLoaded);
			UE_LOG(LogTemp, Log, TEXT("AArenaManager::LoadLevelsWave : Layer loaded successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AArenaManager::LoadLevelsWave : Layer not loaded !! "));
		}
	}
}

void AArenaManager::LoadActorsWave(int CurrentWaveIndex)
{
	FWave currentWaveStruct = Waves[CurrentWaveIndex];

	// I loop in the list of actor to spawn and I will spawn "value" actors of "key" class
	for (const TPair<TSubclassOf<AActor>, int>& Pair : currentWaveStruct.ActorsToSpawn)
	{
		TSubclassOf<AActor> ActorClass = Pair.Key;
		int32 Count = Pair.Value;

		for (int i = 0; i < Count; i++)
		{
			FRotator SpawnRotator = FRotator::ZeroRotator;
			FVector SpawnLocation = FVector::ZeroVector;

			// Spawn deferred to be able to search a valid position on the nav and register the AIPrey if it's one
			AActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(
				ActorClass,
				FTransform(SpawnRotator, SpawnLocation),
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
			);


			if (SpawnedActor)
			{
				// Check if it's an AAIPreyBase
				AAIPreyBase* AIPrey = Cast<AAIPreyBase>(SpawnedActor);  //  A bit expensive
				if (AIPrey)
				{
					// Found associated Navmesh
					const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
					if (NavSys)
					{
						const FNavAgentProperties& AgentProps = AIPrey->GetNavAgentPropertiesRef();
						const ANavigationData* NavDataConst = NavSys->GetNavDataForProps(AgentProps);
						ANavigationData* NavData = const_cast<ANavigationData*>(NavDataConst);

						if (NavData)
						{
							const FVector Origin = NavData->GetActorLocation();
							const float SearchRadius = 1000.f; // Could custom it

							FNavLocation RandomNavLocation;
							if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, RandomNavLocation, NavData))
							{
								SpawnLocation = RandomNavLocation.Location; // Random and Valid Location choosen
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("AArenaManager::LoadActorsWave: Can't found a valid location for this actor %s."), *ActorClass->GetName());
							}
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("AArenaManager::LoadActorsWave: Can't found navdata for this actor %s."), *ActorClass->GetName());
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("AArenaManager::LoadActorsWave: Can't found navsystem for this actor %s."), *ActorClass->GetName());

					}
				}


				//Final Spawn Actor
				SpawnedActor->FinishSpawning(FTransform(SpawnRotator, SpawnLocation));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AArenaManager::LoadActorsWave: Actor can't be spawned"));

			}
		}
	}
}

void AArenaManager::RegisterAIPrey(AAIPreyBase* AIPrey)
{
	if (AIPrey && !AIPreysLoaded.Contains(AIPrey) && !IgnoredElements.Contains(AIPrey->GetClass())) // Check if it's valid and not already registered
	{
		AIPreysLeftArena++;
		AIPreysLeftWave.AddUnique(AIPrey);
		AIPreysLoaded.AddUnique(AIPrey);
		AIPrey->OnCaptured.AddDynamic(this, &AArenaManager::AIPreyCaptured);
		AIPrey->bIsArenaRegistered = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AArenaManager::RegisterAIPrey: Invalid AIPrey provided"));
	}
}



void AArenaManager::AIPreyCaptured(AAIPreyBase* AICaptured)
{
	if (AICaptured)
	{
		//Update counts
		AIPreysLeftArena--;
		int indexAIPrey = -1;
		indexAIPrey = AIPreysLoaded.Find(AICaptured);

		if (indexAIPrey != -1)
		{
			AIPreysLoaded.RemoveAt(indexAIPrey);
		}

		//Check conditions end

		if (IsLastWave())
		{
			VerifEndArenaCondition();
		}
		else
		{
			VerifEndWaveCondition();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AArenaManager::AIPreyCaptured: Invalid AIPrey provided"));
	}

}

bool AArenaManager::IsLastWave()
{
	if (Waves.IsValidIndex(CurrentWave))
	{
		return (Waves[CurrentWave].bIsLastWave || CurrentWave == Waves.Num() - 1);
	}
	return true;
}

void AArenaManager::VerifEndWaveCondition()
{

	GetWorldTimerManager().ClearTimer(TimerWaveCheckCondition);
	if (Waves.IsValidIndex(CurrentWave))
	{
		EArenaWaveEndCondition EndWaveCondition = Waves[CurrentWave].EndWaveCondition;
		switch (EndWaveCondition)
		{

			case EArenaWaveEndCondition::AllWavePreysCaptured :
				if (AIPreysLeftWave.Num() == 0)
				{
					LaunchTimerNextWave();
				}

				// No Need to check for "TimerOnly" becaus it's done when the wave is launched !
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AArenaManager::VerifEndWaveCondition: Invalid wave index provided "));
	}

}

void AArenaManager::LaunchTimerNextWave()
{

	if (Waves.IsValidIndex(CurrentWave))
	{
		if (Waves[CurrentWave].DelayNextWave > 0.0f)
		{
			GetWorldTimerManager().ClearTimer(TimerNextWave);
			GetWorldTimerManager().SetTimer(TimerNextWave, this, &AArenaManager::LaunchNextWave, Waves[CurrentWave].DelayNextWave, false);
			UE_LOG(LogTemp, Log, TEXT("AArenaManager::LaunchTimerNextWave: Timer for next wave activated "));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("AArenaManager::LaunchTimerNextWave: NExt wave immediately launched !"));
			LaunchNextWave();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AArenaManager::LaunchTimerNextWave: No next wave found, launch end !"));
		LaunchTimerEndArena();
	}

}




void AArenaManager::VerifEndArenaCondition()
{

	GetWorldTimerManager().ClearTimer(TimerLastWaveCheckCondition);
	if (!bEndLaunched)
	{
		switch (EndCondition)
		{
		case EArenaEndCondition::TimerOnly:
			LaunchTimerEndArena();
			break;

		case EArenaEndCondition::AllPreysCaptured:
			if (AIPreysLeftArena <= 0)
			{
				LaunchTimerEndArena();
			}
			break;
		}
	}
}

void AArenaManager::LaunchTimerEndArena()
{
	bEndLaunched = true;
	if (DelayAfterEndCondition > 0.0f)
	{
		GetWorldTimerManager().ClearTimer(TimerEnd);
		GetWorldTimerManager().SetTimer(TimerEnd, this, &AArenaManager::EndArena, DelayAfterEndCondition, false);
		UE_LOG(LogTemp, Log, TEXT("AArenaManager::LaunchTimerEndArena: Timer for end arena activated "));

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AArenaManager::LaunchTimerEndArena: End Arena immediately launched !"));
		EndArena();
	}
}

void AArenaManager::EndArena()
{
	UE_LOG(LogTemp, Log, TEXT("AArenaManager::EndArena: Launch End Arena"));
	OnArenaEnd.Broadcast();
}


#if WITH_EDITOR

void AArenaManager::EditLastWave()
{
	for (FWave& Wave : Waves)
	{
		Wave.bIsLastWave = false;
	}

	if (Waves.Num() > 0)
	{
		Waves[Waves.Num() - 1].bIsLastWave = true;
	}
}


void AArenaManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName NAME_PropertyChanged = PropertyChangedEvent.Property->GetFName();
	static const FName NAME_WAVE = FName(TEXT("Waves"));

	if (PropertyChangedEvent.Property)
	{
		if (NAME_PropertyChanged == NAME_WAVE)
		{
			EditLastWave();
		}
	}
}
#endif