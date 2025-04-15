// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "ToolboxBPFL.generated.h"

class ULifeComponent;
class UCaptureGameManager;


UCLASS()
class B12_TEST_LAURELENNSG_API UToolboxBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public : 

	UFUNCTION(BlueprintCallable, Category = "Toolbox")
	static ULifeComponent* GetLifeComponent(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Toolbox")
	static UCaptureGameManager* GetGameManager();
};
