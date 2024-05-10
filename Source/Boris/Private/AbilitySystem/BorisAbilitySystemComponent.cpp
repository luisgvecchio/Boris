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
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;	

	GetAllAbilities(GrantedAbilities);

	for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToRemove)
	{
		FGameplayAbilitySpec AbilitySpecToRemove = FGameplayAbilitySpec(AbilityClass, 1);

		for (const FGameplayAbilitySpecHandle GrantedAbilitySpecHandle : GrantedAbilities)
		{
			FGameplayAbilitySpec* GrantedAbilitySpec = FindAbilitySpecFromHandle(GrantedAbilitySpecHandle);

			if (GrantedAbilitySpec->Ability->GetClass() == AbilitySpecToRemove.Ability.GetClass())
			{
				if (const UBorisGameplayAbility* AbilityToRemove = Cast<UBorisGameplayAbility>(AbilitySpecToRemove.Ability))
				{
					GrantedAbilitySpec->DynamicAbilityTags.RemoveTag(AbilityToRemove->StartupInputTag);
					ClearAbility(GrantedAbilitySpec->Handle);
				}
			}
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
