// Copyright Three Headed Monkey Studios


#include "Character/EnemyCharacter.h"
#include "Boris/Boris.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	

	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Outline
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED_OUTLINE);
	//Abilitysystem Initialization
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
