// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/InteractionComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "B12_Test_LaurelennSg/B12_Test_LaurelennSgCharacter.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Managers/CaptureGameManager.h"
#include "ToolboxBPFL.h"
#include <Kismet/GameplayStatics.h>

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UKismetSystemLibrary::IsValidClass(WidgetclassToShow))
	{
		if (WidgetComp)
		{
			WidgetComp->SetWidgetClass(WidgetclassToShow);
			WidgetComp->InitWidget();

			WidgetComp->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			WidgetComp->SetWidgetSpace(EWidgetSpace::World);
			//WidgetComp->SetTwoSided(true);
			WidgetComp->SetRelativeLocation(WidgetOffset);
			WidgetComp->SetWorldScale3D(WidgetScale);
			WidgetComp->SetDrawAtDesiredSize(true);
			WidgetComp->SetPivot(FVector2D(0.5f, 0.5f));
			
			Widget = WidgetComp->GetWidget();
			if (Widget)
			{
				Widget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	UCaptureGameManager* GameManager = UToolboxBPFL::GetGameManager();

	if (GameManager)
	{
		GameManager->OnGameStateChange.AddDynamic(this, &UInteractionComponent::OnGameStateChange);
	}
	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnEndOverlap);

	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateWidgetRotation();
}

void UInteractionComponent::UpdateWidgetRotation()
{
	if (WidgetComp && Widget && Widget->GetVisibility() == ESlateVisibility::Visible) // Don't make it rotate if we didn't see it
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			FVector CameraLocation = CameraManager->GetCameraLocation();
			FVector ToCamera = CameraLocation - WidgetComp->GetComponentLocation();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(ToCamera).Rotator();

			LookAtRotation.Pitch = 0.0f; // Keep vertical rotation fixed
			LookAtRotation.Roll = 0.0f; // Keep vertical rotation fixed

			WidgetComp->SetWorldRotation(LookAtRotation);
		}
	}
}

void UInteractionComponent::TryInteractWith()
{
	if (bIsEnabled)
	{
		if (bIsActivated)
		{
			DeactivateInteraction();
		}
		else
		{
			ActivateInteraction();
		}
	}
}


void UInteractionComponent::ActivateInteraction()
{
	if (bIsTriggeable)
	{
		bIsActivated = true;
		OnActivation.Broadcast();
		OnActivationWithParams.Broadcast(bHasAlreadyBeenActivated, OverlappingPlayer);
		bHasAlreadyBeenActivated = true;
		if (Widget)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UInteractionComponent::DeactivateInteraction()
{
	if (bIsTriggeable && bCanBeDeactivated)
	{
		bIsActivated = false;
		OnDeactivation.Broadcast();
		OnDeactivationWithParams.Broadcast(bHasAlreadyBeenActivated, OverlappingPlayer);
		bHasAlreadyBeenDeactivated = true;
		if (Widget)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsEnabled)
	{
		if (AB12_Test_LaurelennSgCharacter* Player = Cast<AB12_Test_LaurelennSgCharacter>(OtherActor))
		{
			bIsTriggeable = true;
			OverlappingPlayer = Player;
			OverlappingPlayer->TriggeableInteractComp = this;
			if (Widget)
			{
				Widget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AB12_Test_LaurelennSgCharacter* Player = Cast<AB12_Test_LaurelennSgCharacter>(OtherActor);
	if (Player == OverlappingPlayer && IsValid(OverlappingPlayer))
	{
		bIsTriggeable = false;
		OverlappingPlayer->TriggeableInteractComp = nullptr;
		OverlappingPlayer = nullptr;
		if (Widget)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UInteractionComponent::OnGameStateChange(ECaptureGameState GameState)
{
	if (GameState != ECaptureGameState::Playing)
	{
		Activate(false);
	}
	else
	{
		Activate(true);
	}
}
