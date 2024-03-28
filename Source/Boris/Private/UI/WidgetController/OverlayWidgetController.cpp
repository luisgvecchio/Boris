// Copyright Three Headed Monkey Studios


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "AbilitySystem/BorisAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBorisAttributeSet* BorisAttributeSet = CastChecked<UBorisAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(BorisAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(BorisAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UBorisAttributeSet* BorisAttributeSet = CastChecked<UBorisAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BorisAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BorisAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// For example, say that Tag = Message.HealthPotion
				// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}