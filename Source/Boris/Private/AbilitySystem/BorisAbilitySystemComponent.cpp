// Copyright Three Headed Monkey Studios


#include "AbilitySystem/BorisAbilitySystemComponent.h"

void UBorisAbilitySystemComponent::InitActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBorisAbilitySystemComponent::EffectApplied);
}

void UBorisAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	for (auto Tag : TagContainer)
	{
		//TODO: Broadcast tag to Widget Controller
		const FString Msg = FString::Printf(TEXT("GE TAG : %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
	}
}
