// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "CaptureZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPreyCaptured, int, Score);

class UBoxComponent;
class AAIPreyBase;
class UNiagaraSystem;

UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API ACaptureZone : public AActor
{
	GENERATED_BODY()
#pragma region ----- VARIABLES -----

public :

	/**Actors that can be detected (will give us score on detection in this zone)*/
	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AAIPreyBase>> PreysDetectable;
	
	/**Will spawn where the prey is captured*/
	UPROPERTY(Category = "#Setup", EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* VFXOnCaptured;

	/**Could be customable with several options as an enum for exemple (see readme)*/
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite)
	UBoxComponent* DetectionZone;

	UPROPERTY(BlueprintCallable);
	FOnPreyCaptured OnPreyCaptured;
	// NOTE : We could create an enum to choose what kind of shapeComponent we want to use (sphere, box, staticmesh...) and enable or disable collision following the selection
#pragma endregion



#pragma region ----- FUNCTIONS -----

public:	
	// Sets default values for this actor's properties
	ACaptureZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
	void HandleGameStart();

	UFUNCTION(BlueprintCallable)
	void HandleGameEnd();

	UFUNCTION()
	void OnActorEnteredZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	bool IsDetectableActor(AAIPreyBase* ActorPrey);

	UFUNCTION(BlueprintCallable)
	void OnGameStateChange(ECaptureGameState State);

#pragma endregion

};
