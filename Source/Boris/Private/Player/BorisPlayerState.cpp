// Copyright Three Headed Monkey Studios


#include "Player/BorisPlayerState.h"
#include "Boris/Boris.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "AbilitySystem/BorisAttributeSet.h"
#include "Net/UnrealNetwork.h"

ABorisPlayerState::ABorisPlayerState()
{
	NetUpdateFrequency = NET_UPDATE_FRECUENCY;

	AbilitySystemComponent = CreateDefaultSubobject<UBorisAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBorisAttributeSet>("AttributeSet");
}

void ABorisPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABorisPlayerState, Level);
}

UAbilitySystemComponent* ABorisPlayerState::GetAbilitySystemComponent() const
{
	return GetBorisAbilitySystemComponent();
}

UBorisAbilitySystemComponent* ABorisPlayerState::GetBorisAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABorisPlayerState::OnRep_Level(int32 OldLevel)
{

}