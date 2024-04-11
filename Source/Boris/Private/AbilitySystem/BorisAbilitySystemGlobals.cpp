// Copyright Three Headed Monkey Studios


#include "AbilitySystem/BorisAbilitySystemGlobals.h"

#include "BorisAbilityTypes.h"

FGameplayEffectContext* UBorisAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBorisGameplayEffectContext();
}