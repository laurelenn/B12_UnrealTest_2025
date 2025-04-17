// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CaptureZone.h"
#include "ToolboxBPFL.h"
#include "Managers/CaptureGameManager.h"
#include "Components/BoxComponent.h"
#include "Actors/AI/AIPreyBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Sets default values
ACaptureZone::ACaptureZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DetectionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneDetection"));
	RootComponent = DetectionZone;
	DetectionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetectionZone->OnComponentBeginOverlap.AddDynamic(this, &ACaptureZone::OnActorEnteredZone);
	// Collision Detection settings may be done in Blueprints to be easily customable
}

// Called when the game starts or when spawned
void ACaptureZone::BeginPlay()
{
	Super::BeginPlay();
	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();
	if (GameManager)
	{
		GameManager->OnGameStateChange.AddDynamic(this, &ACaptureZone::OnGameStateChange);
		GameManager->RegisterCaptureZone(this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ACaptureZone::BeginPlay : No Game Manager found !!"));
	}
}


void ACaptureZone::HandleGameStart()
{
	UE_LOG(LogTemp, Log, TEXT("ACaptureZone::HandleGameStart : Activate Collisions"));

	DetectionZone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACaptureZone::HandleGameEnd()
{
	UE_LOG(LogTemp, Log, TEXT("ACaptureZone::HandleGameEnd : DeActivate Collisions"));

	DetectionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ACaptureZone::OnActorEnteredZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAIPreyBase* PreyCaptured = Cast<AAIPreyBase>(OtherActor); // Will be nullptr if it's not a good class
	if (IsDetectableActor(PreyCaptured)) 
	{
		if (VFXOnCaptured)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXOnCaptured, OtherActor->GetActorLocation());
		}
		PreyCaptured->Captured();
		OnPreyCaptured.Broadcast(PreyCaptured->GetScore());
	}
}

bool ACaptureZone::IsDetectableActor(AAIPreyBase* ActorPrey)
{
	if (IsValid(ActorPrey))
	{
		if (UToolboxBPFL::ContainsChildOf(PreysDetectable,ActorPrey->GetClass())) // Check if this actor is in detectable list
		{
			return true;
		}
		UE_LOG(LogTemp, Log, TEXT("ACaptureZone::IsDetectableActor : This actor ( %s ) wasn't in the detectable list !"), *ActorPrey->GetName());
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ACaptureZone::IsDetectableActor : This actor wasn't a child of AIPreyBase class !"));
	}
	return false;
}

void ACaptureZone::OnGameStateChange(ECaptureGameState State)
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



