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

	FEffectAssetTags EffectAssetTags;

	void InitActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void RemoveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToRemove);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
