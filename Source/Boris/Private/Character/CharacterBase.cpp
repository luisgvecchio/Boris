// Copyright Three Headed Monkey Studios


#include "Character/CharacterBase.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

