// Copyright Three Headed Monkey Studios


#include "Player/BorisPlayerState.h"
#include "Boris/Boris.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "AbilitySystem/BorisAttributeSet.h"

ABorisPlayerState::ABorisPlayerState()
{
	NetUpdateFrequency = NET_UPDATE_FRECUENCY;

	AbilitySystemComponent = CreateDefaultSubobject<UBorisAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBorisAttributeSet>("AttributeSet");
}

UBorisAbilitySystemComponent* ABorisPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}