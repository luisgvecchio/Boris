// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BorisDamageGameplayAbility.h"
#include "BorisMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisMeleeAttack : public UBorisDamageGameplayAbility
{
	GENERATED_BODY()


protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void UpdateGameplayEffectForDamage();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartCombo1();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void FinishCombo();
	
};
