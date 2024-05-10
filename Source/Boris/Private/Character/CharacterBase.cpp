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

	HitCapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Collider"));
	HitCapsuleCollider->SetupAttachment(GetRootComponent());
	HitCapsuleCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ACharacterBase::AddCharacterStartUpAbilities()
{
	if (!HasAuthority())
		return;

	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);

	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		CurrentAbilities.Add(AbilityClass);
	}
}

void ACharacterBase::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAdd)
{
	if (!HasAuthority())
		return;

	AbilitySystemComponent->AddCharacterAbilities(AbilitiesToAdd);

	for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToAdd)
	{
		CurrentAbilities.Add(AbilityClass);
	}
}

void ACharacterBase::RemoveGivenCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToRemove)
{
	if (!HasAuthority())
		return;

	AbilitySystemComponent->RemoveCharacterAbilities(AbilitiesToRemove);

	for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToRemove)
	{
		CurrentAbilities.RemoveSingle(AbilityClass);
	}
}

void ACharacterBase::RemoveAllCharacterAbilities()
{
	if (!HasAuthority())
		return;

	AbilitySystemComponent->ClearAllAbilities();

	CurrentAbilities.Reset();
}

FVector ACharacterBase::GetCombatSocketLocation_Implementation()
{
	check(EquippedWeapon->GetMesh());

	return EquippedWeapon->GetMesh()->GetSocketLocation(WeaponTipSocketName);
}

int32 ACharacterBase::GetPlayerLevel()
{
	return int32();
}

void ACharacterBase::SetCollisionTypeForHitCollider(const FName& NewCollisionProfile)
{
	if (HitCapsuleCollider)
	{
		HitCapsuleCollider->SetCollisionProfileName(NewCollisionProfile);
	}
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

void ACharacterBase::SendAbilitySpecHandleToEquippedWeapon(FGameplayEffectSpecHandle IncomingAbilitySpecHandle)
{
	if (CharacterState != ECharacterState::ECS_EquippedWithWeapon)
		return;

	EquippedWeapon->DamageSpecHandle = IncomingAbilitySpecHandle;
}

void ACharacterBase::Die()
{
	if(EquippedWeapon)
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

	if (HitCapsuleCollider)
		HitCapsuleCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

