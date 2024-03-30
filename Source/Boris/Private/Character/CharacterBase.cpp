// Copyright Three Headed Monkey Studios


#include "Character/CharacterBase.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "Components/BoxComponent.h"

//TODO: Erase after debugging
#include "Kismet/KismetSystemLibrary.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponBoxCollider = CreateDefaultSubobject<UBoxComponent>("WeaponBoxCollider");
	WeaponBoxCollider->AttachToComponent(Weapon, FAttachmentTransformRules::KeepRelativeTransform);
}

UBorisAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ACharacterBase::InitializePrimaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) 
		return;

	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

FVector ACharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

int32 ACharacterBase::GetPlayerLevel()
{
	return int32();
}

void ACharacterBase::ActivateWeaponCollider()
{
	if (!WeaponBoxCollider)
		return;

	WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacterBase::DeactivateWeaponCollider()
{
	if (!WeaponBoxCollider)
		return;

	WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

