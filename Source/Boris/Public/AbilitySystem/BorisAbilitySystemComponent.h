// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BorisAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void InitActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);
protected:

	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
