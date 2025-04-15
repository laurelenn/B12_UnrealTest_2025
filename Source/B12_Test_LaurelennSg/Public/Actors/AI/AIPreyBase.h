// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBase.h"
#include "AIPreyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCaptured, AAIPreyBase*, AIPrey);

class UAIPreyDataAsset;
class UHoldableComponent;

UCLASS(Abstract)
class B12_TEST_LAURELENNSG_API AAIPreyBase : public AAIBase
{
	GENERATED_BODY()
	
#pragma region ----- VARIABLES -----

protected : 

	UPROPERTY(Category = "Components", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHoldableComponent* HoldableComponent;

public:

	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAIPreyDataAsset* DataAsset;

	UPROPERTY(Category = Runtime, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsArenaRegistered = false;

	// Events
	FOnCaptured OnCaptured;

	#pragma endregion


#pragma region ----- FUNCTIONS -----

public:	
	// Sets default values for this actor's properties
	AAIPreyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void Captured();

	UFUNCTION(Category = "API", BlueprintCallable)
	int GetScore();

	UFUNCTION(Category = "API", BlueprintCallable)
	UHoldableComponent* GetHoldComp(){return HoldableComponent;}

#pragma endregion

};
