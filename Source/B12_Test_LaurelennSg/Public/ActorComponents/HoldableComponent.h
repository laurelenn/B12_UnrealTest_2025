// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/InteractionComponent.h"
#include "HoldableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeld, UHoldableComponent*, HeldComponent, bool, bIsHeld);


UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API UHoldableComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:

	FOnHeld OnHeld;

#pragma region ----- FUNCTIONS -----
	// Sets default values for this component's properties
	UHoldableComponent();

protected:

	virtual void BeginPlay() override;

	virtual void ActivateInteraction() override;

	virtual void DeactivateInteraction() override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	public : 

	/**Handle hold/drop events
	@param bIsHold : if true, handle "hold" case - if false, handle "drop" case*/
	UFUNCTION(BlueprintCallable)
	void HandleHoldDrop(bool bIsHold);

	#pragma endregion

};
