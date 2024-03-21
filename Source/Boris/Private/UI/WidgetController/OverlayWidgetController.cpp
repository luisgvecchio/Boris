// Copyright Three Headed Monkey Studios


#include "UI/WidgetController/OverlayWidgetController.h"

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

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BorisAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		BorisAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}