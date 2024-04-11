// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BorisGameplayAbility.h"
#include "BorisDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisDamageGameplayAbility : public UBorisGameplayAbility
{
	GENERATED_BODY()	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
