// Copyright Three Headed Monkey Studios


#include "AbilitySystem/Abilities/BorisDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/EnemyCharacter.h"
#include "Actor/Items/Weapons/WeaponBase.h"

void UBorisDamageGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UpdateDamageSpecHandle();
}

void UBorisDamageGameplayAbility::UpdateDamageSpecHandle()
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());

	Character->SendAbilitySpecHandleToEquippedWeapon(DamageSpecHandle);
	Character->ActivateWeaponCollider();
}

void UBorisDamageGameplayAbility::StartAttack()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
		return;

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());

	if (!Character)
		return;

	Character->ActivateWeaponCollider();
}

void UBorisDamageGameplayAbility::FinishAtttack()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
		return;

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());

	if (!Character)
		return;

	Character->DeactivateWeaponCollider();
	Character->GetEquippedWeapon()->ResetActorsToIgnore();
}