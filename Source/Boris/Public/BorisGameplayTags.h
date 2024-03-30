// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * BorisGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FBorisGameplayTags
{
public:
	static const FBorisGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Vitality;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Wisdom;

	FGameplayTag Attributes_Secondary_Accuracy;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_AttackCooldownRecovery;
	FGameplayTag Attributes_Secondary_AttackSpeed;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_Endurance;
	FGameplayTag Attributes_Secondary_Health;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MovementSpeed;
	FGameplayTag Attributes_Secondary_Stamina;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

protected:

private:
	static FBorisGameplayTags GameplayTags;
};
