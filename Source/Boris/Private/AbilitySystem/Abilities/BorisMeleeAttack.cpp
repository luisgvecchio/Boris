// Copyright Three Headed Monkey Studios


#include "AbilitySystem/Abilities/BorisMeleeAttack.h"

#include "AbilitySystemComponent.h"
#include "Character/CharacterBase.h"
#include "BorisGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Actor/Items/Weapons/WeaponBase.h"


void UBorisMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UpdateDamageSpecHandle();
}

void UBorisMeleeAttack::UpdateDamageSpecHandle()
{
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Character->GetEquippedWeapon());

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 0, EffectContextHandle);

	FBorisGameplayTags GameplayTags = FBorisGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	Character->SendAbilitySpecHandleToEquippedWeapon(SpecHandle);
}



