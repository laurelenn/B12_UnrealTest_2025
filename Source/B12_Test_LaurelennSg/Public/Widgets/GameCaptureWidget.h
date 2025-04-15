// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums.h"
#include "GameCaptureWidget.generated.h"

class UCaptureGameManager;

UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UGameCaptureWidget : public UUserWidget
{

	GENERATED_BODY()

	protected : 
	friend class UCaptureGameManager;


	UPROPERTY(Category = Runtime, BlueprintReadWrite)
	int Score = 0;

	UPROPERTY(Category = Runtime, BlueprintReadWrite)
	float CurrentTimer = 0.f;

	UPROPERTY(Category = Runtime, BlueprintReadWrite)
	ECaptureGameState CurrentGameState = ECaptureGameState::Initialization;



	/** Will init the widget and will register to the gameStateChanged event*/
	UFUNCTION()
	void InitWidget(UCaptureGameManager* GameManager);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateScore(int NewScore);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateTime(float NewTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateUIGameState(ECaptureGameState NewGameState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisplayGameOverPanel(ECaptureGameOverReason Reason);

};
