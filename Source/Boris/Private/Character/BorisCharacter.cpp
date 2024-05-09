// Copyright Three Headed Monkey Studios


#include "Character/BorisCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/BorisPlayerState.h"
#include "Player/BorisPlayerController.h"
#include "UI/HUD/BorisHUD.h"
#include "Components/BoxComponent.h"
#include "Actor/Items/Weapons/WeaponBase.h"



ABorisCharacter::ABorisCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 450.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Trace channel for PlayerAttack overlaps.
	WeaponAttackOverlapChannel = ECollisionChannel::ECC_GameTraceChannel2;

	WeaponAttackCollisionProfile = FName("PlayerAttackOverlap");

	//Collision for HitCollider overlaps.
	HitColliderCollisionProfile = FName("GetHitFromEnemy");
	SetCollisionTypeForHitCollider(HitColliderCollisionProfile);
}

void ABorisCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();

	AddCharacterAbilities();

}

void ABorisCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void ABorisCharacter::InitAbilityActorInfo()
{
	ABorisPlayerState* BorisPlayerState = GetPlayerState<ABorisPlayerState>();
	check(BorisPlayerState);
	AbilitySystemComponent = Cast<UBorisAbilitySystemComponent>(BorisPlayerState->GetAbilitySystemComponent());

	AbilitySystemComponent->InitActorInfo(BorisPlayerState, this);

	AttributeSet = BorisPlayerState->GetAttributeSet();

	if (ABorisPlayerController* BorisPlayerController = Cast<ABorisPlayerController>(GetController()))
	{
		if (ABorisHUD* BorisHUD = Cast<ABorisHUD>(BorisPlayerController->GetHUD()))
		{
			BorisHUD->InitOverlay(BorisPlayerController, BorisPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}

void ABorisCharacter::EquipWeapon(AWeaponBase* Weapon)
{
	if (!Weapon)
		return;

	EquippedWeapon = Weapon;
	DeactivateWeaponCollider();

	Weapon->Equip(GetMesh(), FName("WeaponHandSocket"), this, this, WeaponAttackCollisionProfile);

	CharacterState = ECharacterState::ECS_EquippedWithWeapon;
}

void ABorisCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("WeaponHandSocket"));
	}
}


int32 ABorisCharacter::GetPlayerLevel()
{
	const ABorisPlayerState* BorisPlayerState = GetPlayerState<ABorisPlayerState>();
	check(BorisPlayerState);
	return BorisPlayerState->GetPlayerLevel();
}
