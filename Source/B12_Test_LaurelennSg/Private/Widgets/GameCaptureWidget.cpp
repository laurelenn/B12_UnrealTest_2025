// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/GameCaptureWidget.h"
#include "Managers/CaptureGameManager.h"

void UGameCaptureWidget::InitWidget(UCaptureGameManager* GameManager)
{
	if (IsValid(GameManager))
	{
		GameManager->OnGameStateChange.AddDynamic(this, &UGameCaptureWidget::UpdateUIGameState);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UGameCaptureWidget::InitWidget : Invalid GameManager provided !"));
	}
}

void UGameCaptureWidget::UpdateScore_Implementation(int NewScore)
{
	Score = NewScore;
}

void UGameCaptureWidget::UpdateTime_Implementation(float NewTime)
{
	CurrentTimer = NewTime;
}

void UGameCaptureWidget::UpdateUIGameState_Implementation(ECaptureGameState NewGameState)
{
	CurrentGameState = NewGameState;
}

void UGameCaptureWidget::DisplayGameOverPanel_Implementation(ECaptureGameOverReason Reason)
{
 // Overriden in BP
}

