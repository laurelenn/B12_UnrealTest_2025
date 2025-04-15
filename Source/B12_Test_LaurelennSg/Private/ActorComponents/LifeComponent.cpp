// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/LifeComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{

// Tick not relevant here
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULifeComponent::InitStartParams(float StartLifeValue, float MinLifeValue, float MaxLifeValue)
{
	StartLife = StartLifeValue;
	MinLife = MinLifeValue;
	MaxLife = MaxLifeValue;
}


// Called when the game starts
void ULifeComponent::BeginPlay()
{

	SetCurrentLife(StartLife, nullptr);		
	Owner = GetOwner();
	Super::BeginPlay();
	
}

bool ULifeComponent::SetCurrentLife(const float Life, AActor* Causer)
{
	// Clamp new life between min and max
	const float NewLife = FMath::Clamp(Life, MinLife, MaxLife);

	// Check the difference between the current life and the new life
	float Amount = CurrentLife > NewLife ? (UKismetMathLibrary::Abs(CurrentLife - NewLife) * -1.f) : (UKismetMathLibrary::Abs(CurrentLife - NewLife));

	
	// Don't do anything if there is no change
	if (FMath::IsNearlyEqual(NewLife, CurrentLife))
	{
		return false;
	}

	// Edit new life and call event
	CurrentLife = NewLife;
	OnLifeChanged.Broadcast(this, Amount, Amount<0,Causer);

	// DEATH - Check if we should die and call death event
	bIsDead = CheckShouldDie();
	
	if (bIsDead)
	{
		OnDeath.Broadcast(this, Causer);
	}


	return true;
}

bool ULifeComponent::SetMaxLife(const float Life, AActor* Causer)
{
	// Avoid to get a MaxLife lower that MinLife ! 
	MaxLife = FMath::Max(Life, MinLife);
	return true;
}

bool ULifeComponent::SetMinLife(const float Life, AActor* Causer)
{
	// Avoid to get MinLife Lower than 0 and bigger that MaxLife ! 
	MinLife = FMath::Clamp(Life, 0.f, MaxLife);
	return true;
}

bool ULifeComponent::SetInvincible(const bool bState, AActor* Causer)
{
	if (bIsDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULifeComponent::SetInvincible : The lifeComp of %s is already dead ! Nothing done"), IsValid(Owner) ? *Owner->GetName() : TEXT("Unknown Owner"));
		return false;
	}

	if (bState == bIsInvincible)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULifeComponent::SetInvincible : The lifeComp of %s is already on this state ! Nothing done"), IsValid(Owner) ? *Owner->GetName() : TEXT("Unknown Owner"));
		return false;
	}

	bIsInvincible = bState;
	OnSetInvincible.Broadcast(this, bIsInvincible, Causer);


	if (CheckShouldDie())
	{
		TryKill(false, Causer);
	}

	return true;
}




bool ULifeComponent::Damage(const float Amount, AActor* Causer)
{

	// Amount is negative, it's a heal
	if (Amount < 0.0f)
	{
		return Heal(-Amount, Causer);
	}

	if (bIsDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULifeComponent::Damage : The lifeComp of %s is already dead ! Nothing done"), IsValid(Owner) ? *Owner->GetName() : TEXT("Unknown Owner"));
		return false;
	}

	if (bIsInvincible)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULifeComponent::Damage : The lifeComp of %s is invincible ! Nothing done"), IsValid(Owner) ? *Owner->GetName() : TEXT("Unknown Owner"));
		return false;
	}

	return SetCurrentLife(CurrentLife - Amount, Causer);
}

bool ULifeComponent::Heal(const float Amount, AActor* Causer)
{

	// Amount is negative, it's a damage
	if (Amount < 0.0f)
		return Damage(-Amount, Causer);

	if (bIsDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULifeComponent::Heal : The lifeComp of %s is already dead ! Nothing done"), IsValid(Owner) ? *Owner->GetName() : TEXT("Unknown Owner"));
		return false;

	}

	return SetCurrentLife(CurrentLife + Amount, Causer);
}

bool ULifeComponent::TryKill(const bool bForceKill, AActor* Causer)
{
	if (bIsDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULifeComponent::SetInvincible : The lifeComp of %s is already dead ! Nothing done."), IsValid(Owner) ? *Owner->GetName() : TEXT("Unknown Owner"));
		return false;
	}
	
	if (bIsInvincible)
	{
		// if ForceKill = true, Became vulnerable before beeing killed
		if (bForceKill)
		{
			SetInvincible(false, Causer);
		}
		else 
		{
			return false;
		}
	}

	return SetCurrentLife(0.f, Causer);
}

bool ULifeComponent::CheckShouldDie()
{
	return CurrentLife <= 0.0f && !bIsInvincible;

}

