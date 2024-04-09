// Copyright Three Headed Monkey Studios


#include "Character/EnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Boris/Boris.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "AbilitySystem/BorisBlueprintFunctionLibrary.h"
#include "AbilitySystem/BorisAttributeSet.h"
#include "UI/Widgets/BorisUserWidget.h"


AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UBorisAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);	

	AttributeSet = CreateDefaultSubobject<UBorisAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Outline
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED_OUTLINE);

	//Abilitysystem Initialization
	GetAbilitySystemComponent()->InitActorInfo(this, this);

	//AttributesInitialization
	InitializeDefaultAttributes();

	if (UBorisUserWidget* BorisUserWidget = Cast<UBorisUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		BorisUserWidget->SetWidgetController(this);
	}

	if (const UBorisAttributeSet* BorisAS = Cast<UBorisAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BorisAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BorisAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(BorisAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BorisAS->GetMaxHealth());
	}
}

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);

	if (!WeaponMesh)
		return;

	WeaponMesh->SetRenderCustomDepth(true);
}

void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);

	if (!WeaponMesh)
		return;

	WeaponMesh->SetRenderCustomDepth(false);
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UBorisAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}
