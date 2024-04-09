// Copyright Three Headed Monkey Studios


#include "BorisGameplayTags.h"
#include "GameplayTagsManager.h"

FBorisGameplayTags FBorisGameplayTags::GameplayTags;

void FBorisGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases Physical Damage and Endurance.")
	);

	GameplayTags.Attributes_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vitality"),
		FString("Increases Life and Stamina")
	);

	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"),
		FString("Increases Range Damage, Accuracy and Critical Hit Damage")
	);

	GameplayTags.Attributes_Primary_Wisdom = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Wisdom"),
		FString("Increases Critical Hit Chance, Resistance to Status Modifiers and faster Attack Cooldown Recovery.")
	);

	/*
	 * Secondary Attributes
	 */

	GameplayTags.Attributes_Secondary_Accuracy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Accuracy"),
		FString("reduces chance of missing an attack hit")
	);

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces Physical Damage taken")
	);

	GameplayTags.Attributes_Secondary_AttackCooldownRecovery = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.AttackCooldownRecovery"),
		FString("Percentage of recovery for the cooldown of the attacks")
	);

	GameplayTags.Attributes_Secondary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.AttackSpeed"),
		FString("reduces execution time between attacks")
	);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Probability of landing a Critical Hit")
	);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Damage multiplier for Critical Hits")
	);

	GameplayTags.Attributes_Secondary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Endurance"),
		FString("Reduces probability of being affected by a Perfect or Critical Hit")
	);

	GameplayTags.Attributes_Secondary_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Health"),
		FString("Current Amount of Health")
	);

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
	);

	GameplayTags.Attributes_Secondary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MovementSpeed"),
		FString("How fast the player can Run")
	);

	GameplayTags.Attributes_Secondary_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Stamina"),
		FString("The amount of stamine determines how much can a player Run without getting tired")
	);

	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
	);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
	);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
	);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
	);

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);
}
