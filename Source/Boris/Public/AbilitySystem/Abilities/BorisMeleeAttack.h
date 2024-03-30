// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BorisGameplayAbility.h"
#include "BorisMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisMeleeAttack : public UBorisGameplayAbility
{
	GENERATED_BODY()


protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartCombo1();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void FinishCombo();
	
};
