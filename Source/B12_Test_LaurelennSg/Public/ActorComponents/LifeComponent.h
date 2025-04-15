// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"


/* ***** NOTE *****
 We can add this component on everything working with a life logic, even if this is not IA or Pawn. (Gauges, objects to destroy...)
 We can combine his behaviour by overriding it with other components or features (example : combine it with a statsComponent handling DEF, weaknesses...)
 We could add more features as ressurection with condition, temporary invincibility with previous causer etc...
*/


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLifeChanged, ULifeComponent*, LifeComponent, float, Amount, bool, IsDamage, AActor*, Causer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, ULifeComponent*, LifeComponent, AActor*, Causer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSetInvincible, ULifeComponent*, LifeComponent, bool, bState, AActor*, Causer);


UCLASS(Blueprintable)
class B12_TEST_LAURELENNSG_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()


#pragma region ----- VARIABLES -----
public:

	UPROPERTY(Category = "#Setup", EditAnywhere, meta = (Tooltip = "The life at begin play, must be between minLife and maxLife"))
	float StartLife = 100.f;

	UPROPERTY(Category = "#Setup", EditAnywhere, meta = (Tooltip = "The life can't exceed this max value"))
	float MaxLife = 100.f;

	UPROPERTY(Category = "#Setup", EditAnywhere, meta = (Tooltip = "The life can't exceed this min value (in negative)"))
	float MinLife = 0.f;

	UPROPERTY(Category = Runtime, VisibleAnywhere)
	float CurrentLife = 100.f;

	UPROPERTY(Category = Runtime, EditAnywhere, meta = (Tooltip = "Won't die or be damaged"))
	bool bIsInvincible = false;

	UPROPERTY(Category = Runtime, VisibleAnywhere)
	bool bIsDead = false;

	UPROPERTY(Category = Runtime, VisibleAnywhere)
	AActor* Owner;

	UPROPERTY(Category = "#Debug", EditAnywhere, meta = (Tooltip = "Value for damage debug"))
	float DamageDebugAmount = 10.f;

	UPROPERTY(Category = "#Debug", EditAnywhere, meta = (Tooltip = "Value for damage heal"))
	float HealDebugAmount = 10.f;

	#pragma region Events

	UPROPERTY(Category = "API", BlueprintAssignable)
	FOnLifeChanged OnLifeChanged;	// OnLifeChangedParams(ULifeComponent* LifeComponent, float Amount, bool IsDamage, AActor* Causer). Amount < 0 if we lost life, Amount > 0 if we gained life

	/** Death*/
	UPROPERTY(Category = "Life Component | API", BlueprintAssignable)
	FOnDeath OnDeath; // OnDeathParams(ULifeComponent* LifeComponent, AActor* Causer)

	UPROPERTY(Category = "Life Component | API", BlueprintAssignable)
	FOnSetInvincible OnSetInvincible; // OnSetInvincible(ULifeComponent* LifeComponent, bool bState, AActor* Causer)

	#pragma endregion


#pragma endregion


#pragma region ----- FUNCTIONS ------

public:	
	// Sets default values for this component's properties
	ULifeComponent();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public : 
	#pragma region API

	UFUNCTION(Category = "API", BlueprintCallable)
	void InitStartParams(float StartLifeValue, float MinLifeValue, float MaxLifeValue);

		#pragma region Getters

	UFUNCTION(Category = "API", BlueprintCallable)
	FORCEINLINE float GetStartLife() const { return StartLife; }

		UFUNCTION(Category = "API", BlueprintCallable)
		FORCEINLINE float GetCurrentLife() const { return CurrentLife; }

		UFUNCTION(Category = "API", BlueprintCallable)
		FORCEINLINE float GetMaxLife() const { return MaxLife; }

		UFUNCTION(Category = "API", BlueprintCallable)
		FORCEINLINE float GetMinLife() const { return MinLife; }

		UFUNCTION(Category = "API", BlueprintCallable)
		FORCEINLINE bool IsInvincible() const { return bIsInvincible; }

		UFUNCTION(Category = "API", BlueprintCallable)
		FORCEINLINE bool IsDead() const { return bIsDead; }

		UFUNCTION(Category = "API", BlueprintCallable)
		FORCEINLINE bool IsAlive() const { return !bIsDead; }

		#pragma endregion

		#pragma region Setters

		/** In case you want to set a specific life value, instead of damaging / healing a certain amount.Cannot go under MinLife or over MaxLife. Doesn't work if dead, see @Resurrect*/
		UFUNCTION(Category = "API", BlueprintCallable)
		bool SetCurrentLife(const float Life, AActor* Causer = nullptr);

		/** Cannot go under MinLife
		@param bAlsoSetCurrentLife : Will set CurrentLife at this value too */
		UFUNCTION(Category = "API", BlueprintCallable)
		bool SetMaxLife(const float Life, AActor* Causer = nullptr);

		/** Cannot go over MaxLife. WARNING: Death will always be triggered when CurrentLife reaches 0,
		@param bAlsoSetCurrentLife : Will set CurrentLife at this value too */
		UFUNCTION(Category = "API", BlueprintCallable)
		bool SetMinLife(const float Life, AActor* Causer = nullptr);

		/** Can cause the death if CurrentLife was 0 when unset invincible. Doesn't work if dead*/
		UFUNCTION(Category = "API", BlueprintCallable)
		bool SetInvincible(const bool bState, AActor* Causer = nullptr);

		#pragma endregion


	/** WARNING : If Amount < 0, it will Heal ! */
	UFUNCTION(Category = "API", BlueprintCallable)
	bool Damage(const float Amount, AActor* Causer = nullptr);

	/** WARNING : If Amount < 0, it will Damage ! */
	UFUNCTION(Category = "API", BlueprintCallable)
	bool Heal(const float Amount, AActor* Causer = nullptr);

	/** Will try to kill the lifeComp by setting the life to 0
	@param bForceKill : if true will disable the invincibility */
	UFUNCTION(Category = "API", BlueprintCallable)
	bool TryKill(const bool bForceKill = false, AActor* Causer = nullptr);



	/** Check if life is low and not invincible*/
	UFUNCTION(Category = "API", BlueprintCallable)
	bool CheckShouldDie();

	#pragma endregion


	#pragma region DEBUG

	/** Force kill even if the actor is invincible, usefull for debug */
	UFUNCTION(Category = "#Debug", CallInEditor)
	void ForceKill() {  TryKill(true, nullptr); }

	/** Damage of 10 for debug */
	UFUNCTION(Category = "#Debug", CallInEditor)
	void DamageDebug() {  Damage(DamageDebugAmount, nullptr); }

	/** Damage of 100 for debug */
	UFUNCTION(Category = "#Debug", CallInEditor)
	void HealDebug() {  Heal(HealDebugAmount, nullptr); }

	/** Make the player invincible, can't be damaged anymore*/
	UFUNCTION(Category = "#Debug", BlueprintCallable, CallInEditor)
	void MakeInvincible() {  SetInvincible(true, nullptr); }

	/** Make the player vulnerable, can be damaged*/
	UFUNCTION(Category = "#Debug", BlueprintCallable, CallInEditor)
	void MakeVulnerable() {  SetInvincible(false, nullptr); }

	#pragma endregion


#pragma endregion



};
