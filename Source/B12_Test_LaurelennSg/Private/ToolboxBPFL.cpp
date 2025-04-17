#include "ToolboxBPFL.h"
#include "..\B12_Test_LaurelennSgGameMode.h"
#include "ActorComponents/LifeComponent.h"
#include "Kismet/GameplayStatics.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolboxBPFL.h"

ULifeComponent* UToolboxBPFL::GetLifeComponent(AActor* Actor)
{
    if (IsValid(Actor))
    {
        return Actor->FindComponentByClass<ULifeComponent>();
    }
    return nullptr;
}

UCaptureGameManager* UToolboxBPFL::GetGameManager()
{
    AB12_Test_LaurelennSgGameMode* GameMode = Cast<AB12_Test_LaurelennSgGameMode>(UGameplayStatics::GetGameMode(GWorld));

    if (GameMode)
    {
        return GameMode->GameManager;
    }

    return nullptr;
}

