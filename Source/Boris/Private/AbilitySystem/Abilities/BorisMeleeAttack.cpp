// Copyright Three Headed Monkey Studios


#include "AbilitySystem/Abilities/BorisMeleeAttack.h"

#include "Character/BorisCharacter.h"

//TODO: Erase After debugging
#include "Kismet/KismetSystemLibrary.h"


void UBorisMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
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




