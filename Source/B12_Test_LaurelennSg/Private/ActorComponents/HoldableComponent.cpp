// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/HoldableComponent.h"
#include "Blueprint/UserWidget.h"
#include "B12_Test_LaurelennSg/B12_Test_LaurelennSgCharacter.h"

// Sets default values for this component's properties
UHoldableComponent::UHoldableComponent()
{
}


void UHoldableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHoldableComponent::ActivateInteraction()
{
	Super::ActivateInteraction();

	if (OverlappingPlayer)
	{
		OverlappingPlayer->TryHoldActor(this);
	}
}

void UHoldableComponent::DeactivateInteraction()
{
	Super::DeactivateInteraction();

	if (OverlappingPlayer)
	{
		OverlappingPlayer->TryDropActorHeld(this);
	}
}

void UHoldableComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	AB12_Test_LaurelennSgCharacter* Player = Cast<AB12_Test_LaurelennSgCharacter>(OtherActor);
	if (Player == OverlappingPlayer && IsValid(OverlappingPlayer) && !bIsActivated) // Only if he's not hold !
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



void UHoldableComponent::HandleHoldDrop(bool bIsHold)
{
	GetOwner()->SetActorEnableCollision(!bIsHold);
}



