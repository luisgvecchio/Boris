// Copyright Three Headed Monkey Studios


#include "Character/CharacterBase.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UBorisAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ACharacterBase::AddCharacterAbilities()
{
	UBorisAbilitySystemComponent* BorisASC = AbilitySystemComponent;
	if (!HasAuthority()) 
		return;

	BorisASC->AddCharacterAbilities(StartupAbilities);
}

