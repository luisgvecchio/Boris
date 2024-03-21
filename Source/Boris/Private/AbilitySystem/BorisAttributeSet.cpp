// Copyright Three Headed Monkey Studios


#include "AbilitySystem/BorisAttributeSet.h"
#include "Net/UnrealNetwork.h"


UBorisAttributeSet::UBorisAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
}

void UBorisAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBorisAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBorisAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UBorisAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBorisAttributeSet, Health, OldHealth)
}

void UBorisAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBorisAttributeSet, MaxHealth, OldMaxHealth)

}
