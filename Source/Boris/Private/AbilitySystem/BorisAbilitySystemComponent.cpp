// Copyright Three Headed Monkey Studios

#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "AbilitySystem/BorisGameplayAbility.h"
#include "BorisGameplayTags.h"

void UBorisAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UBorisGameplayAbility* BorisAbility = Cast<UBorisGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(BorisAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UBorisAbilitySystemComponent::RemoveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToRemove)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToRemove)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UBorisGameplayAbility* BorisAbility = Cast<UBorisGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.RemoveTag(BorisAbility->StartupInputTag);
			ClearAbility(AbilitySpec.Handle);
		}
	}
}

void UBorisAbilitySystemComponent::InitActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBorisAbilitySystemComponent::ClientEffectApplied);

	const FBorisGameplayTags& GameplayTags = FBorisGameplayTags::Get();
}

void UBorisAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

void UBorisAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void UBorisAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}
