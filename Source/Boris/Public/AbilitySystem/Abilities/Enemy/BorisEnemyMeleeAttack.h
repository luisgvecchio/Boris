// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BorisDamageGameplayAbility.h"
#include "BorisEnemyMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisEnemyMeleeAttack : public UBorisDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
