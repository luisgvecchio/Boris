// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BorisAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

/**
 * 
 */
UCLASS()
class BORIS_API UBorisAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void InitActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);

	FEffectAssetTags EffectAssetTags;

protected:

	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
