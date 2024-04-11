// Copyright Three Headed Monkey Studios
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "BorisPlayerController.generated.h"


struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UBorisInputConfig;
class UBorisAbilitySystemComponent;
class UDamageTextComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class BORIS_API ABorisPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABorisPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
		UPROPERTY(EditAnywhere, Category = "Input");
		TObjectPtr<UInputMappingContext> BorisContext;	

		UPROPERTY(EditAnywhere, Category = "Input");
		TObjectPtr<UInputAction> MoveAction;

		UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> ShiftAction;

		void ShiftPressed() { bShiftKeyDown = true; };
		void ShiftReleased() { bShiftKeyDown = false; };
		bool bShiftKeyDown = false;

		void Move(const FInputActionValue& InputActionValue);

		void CursorTrace();
		void AutoRun();

		IEnemyInterface* LastActor;
		IEnemyInterface* CurrentActor;

		FHitResult CursorHit;

		void AbilityInputTagPressed(FGameplayTag InputTag);
		void AbilityInputTagReleased(FGameplayTag InputTag);
		void AbilityInputTagHeld(FGameplayTag InputTag);

		UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UBorisInputConfig> InputConfig;

		UPROPERTY()
		TObjectPtr<UBorisAbilitySystemComponent> BorisAbilitySystemComponent;

		UBorisAbilitySystemComponent* GetASC();

		FVector CachedDestination = FVector::ZeroVector;
		float FollowTime = 0.f;
		float ShortPressThreshold = 0.5f;
		bool bAutoRunning = false;
		bool bTargeting = false;

		UPROPERTY(EditDefaultsOnly)
		float AutoRunAcceptanceRadius = 50.f;

		UPROPERTY(VisibleAnywhere)
		TObjectPtr<USplineComponent> Spline;

		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
