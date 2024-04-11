// Copyright Three Headed Monkey Studios


#include "AbilitySystem/Abilities/BorisMeleeAttack.h"

#include "AbilitySystemComponent.h"
#include "Character/BorisCharacter.h"
#include "BorisGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Actor/Items/Weapons/WeaponBase.h"


//TODO: Erase After debugging
#include "Kismet/KismetSystemLibrary.h"


void UBorisMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UpdateGameplayEffectForDamage();
}

void UBorisMeleeAttack::UpdateGameplayEffectForDamage()
{
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	ABorisCharacter* BorisCharacter = Cast<ABorisCharacter>(GetAvatarActorFromActorInfo());

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(BorisCharacter->GetEquippedWeapon());

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 0, EffectContextHandle);

	FBorisGameplayTags GameplayTags = FBorisGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	BorisCharacter->SendAbilitySpecHandleToEquippedWeapon(SpecHandle);
}

void UBorisMeleeAttack::StartCombo1()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
		return;

	ABorisCharacter* BorisCharacter = Cast<ABorisCharacter>(GetAvatarActorFromActorInfo());

	if (!BorisCharacter)
		return;

	BorisCharacter->ActivateWeaponCollider();
}

void UBorisMeleeAttack::FinishCombo()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
		return;

	ABorisCharacter* BorisCharacter = Cast<ABorisCharacter>(GetAvatarActorFromActorInfo());

	if (!BorisCharacter)
		return;	

	BorisCharacter->DeactivateWeaponCollider();
}




