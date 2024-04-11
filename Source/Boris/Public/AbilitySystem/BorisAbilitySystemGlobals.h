// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "BorisAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
