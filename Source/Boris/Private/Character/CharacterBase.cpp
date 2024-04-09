// Copyright Three Headed Monkey Studios


#include "Character/CharacterBase.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "Actor/Items/Weapons/WeaponBase.h"
#include "Components/BoxComponent.h"


//TODO: Erase after not needing debugging
#include "Kismet/KismetSystemLibrary.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	EquippedWeapon = CreateDefaultSubobject<AWeaponBase>("Weapon");

	if (!EquippedWeapon)
		return;

	EquippedWeapon->GetMesh()->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
}

UBorisAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ACharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) 
		return;

	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

FVector ACharacterBase::GetCombatSocketLocation()
{
	check(EquippedWeapon->GetMesh());

	return EquippedWeapon->GetMesh()->GetSocketLocation(WeaponTipSocketName);
}

int32 ACharacterBase::GetPlayerLevel()
{
	return int32();
}

void ACharacterBase::ActivateWeaponCollider()
{
	if (!EquippedWeapon->GetWeaponBoxCollider())
		return;

	EquippedWeapon->GetWeaponBoxCollider()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacterBase::DeactivateWeaponCollider()
{
	if (!EquippedWeapon->GetWeaponBoxCollider())
		return;

	EquippedWeapon->GetWeaponBoxCollider()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

