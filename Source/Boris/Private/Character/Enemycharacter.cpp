// Copyright Three Headed Monkey Studios


#include "Character/Enemycharacter.h"
#include "Boris/Boris.h"

AEnemycharacter::AEnemycharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AEnemycharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED_OUTLINE);
}

void AEnemycharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AEnemycharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
