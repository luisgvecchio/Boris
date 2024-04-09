// Copyright Three Headed Monkey Studios


#include "AbilitySystem/Abilities/BorisMeleeAttack.h"

#include "AbilitySystemComponent.h"
#include "Character/BorisCharacter.h"
#include "BorisGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"


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

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 0, SourceASC->MakeEffectContext());

	FBorisGameplayTags GameplayTags = FBorisGameplayTags::Get();

	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);

	ABorisCharacter* BorisCharacter = Cast<ABorisCharacter>(GetAvatarActorFromActorInfo());
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




