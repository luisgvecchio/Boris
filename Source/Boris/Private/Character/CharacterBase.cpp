// Copyright Three Headed Monkey Studios


#include "Character/CharacterBase.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "Actor/Items/Weapons/WeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"


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

UAnimMontage* ACharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
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
	if (!HasAuthority())
		return;

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

void ACharacterBase::Die()
{
	EquippedWeapon->GetMesh()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

/// <summary>
/// Happens in both Server and Clients
/// </summary>
void ACharacterBase::MulticastHandleDeath_Implementation()
{
	auto WeaponMesh = EquippedWeapon->GetMesh();

	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	auto CharacterMesh = GetMesh();

	CharacterMesh->SetSimulatePhysics(true);
	CharacterMesh->SetEnableGravity(true);
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CharacterMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UCapsuleComponent* HitCollider = Cast<UCapsuleComponent>(GetDefaultSubobjectByName(TEXT("HitCollider")));

	if(HitCollider)
		HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DissolveMaterials();
}

void ACharacterBase::DissolveMaterials()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		EquippedWeapon->GetMesh()->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

