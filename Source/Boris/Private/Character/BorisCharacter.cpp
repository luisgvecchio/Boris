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
#include "Inventory/InventoryBaseComponent.h"


ABorisCharacter::ABorisCharacter()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryBaseComponent>(TEXT("Inventory component"));

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

	AddCharacterStartUpAbilities();

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

	UnequipCurrentEquippedWeapon();

	EquippedWeapon = Weapon;
	DeactivateWeaponCollider();

	Weapon->Equip(GetMesh(), FName("WeaponHandSocket"), this, this, WeaponAttackCollisionProfile);

	//Add Character Abilities after equipping the new Weapon
	AddCharacterAbilities(EquippedWeapon->GetWeaponAbilitiesForPlayers());

	CharacterState = ECharacterState::ECS_EquippedWithWeapon;
}

void ABorisCharacter::UnequipCurrentEquippedWeapon()
{
	if (!EquippedWeapon)
		return;

	//Remove the Abilities of the last EquippedWeapon if any
	RemoveGivenCharacterAbilities(EquippedWeapon->GetWeaponAbilitiesForPlayers());

	EquippedWeapon->GetMesh()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	auto WeaponMesh = EquippedWeapon->GetMesh();

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetSimulatePhysics(true);

	//Timer to activate the sphere collider to be able to pick up the weapon again
	GetWorldTimerManager().SetTimer(EquippedWeapon->SphereCollisionTimer, EquippedWeapon, &AWeaponBase::EnableSphereCollision, 2, false);

	EquippedWeapon = nullptr;
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
