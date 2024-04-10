// Copyright Three Headed Monkey Studios


#include "Character/EnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Boris/Boris.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "AbilitySystem/BorisBlueprintFunctionLibrary.h"
#include "AbilitySystem/BorisAttributeSet.h"
#include "UI/Widgets/BorisUserWidget.h"
#include "BorisGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

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
		AddLambdaListenerToAttributeChange(OnHealthChanged, BorisAS->GetHealthAttribute());
		AddLambdaListenerToAttributeChange(OnMaxHealthChanged, BorisAS->GetMaxHealthAttribute());

		/*AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BorisAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);*/
		/*AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BorisAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);*/

		ListenToHitReactTagChange();

		OnHealthChanged.Broadcast(BorisAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BorisAS->GetMaxHealth());
	}
}
void AEnemyCharacter::AddLambdaListenerToAttributeChange(FOnAttributeChangedSignature& TargetAttributeChangeSignature,FGameplayAttribute EventToListenTo)
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EventToListenTo).AddLambda(
		[&](const FOnAttributeChangeData& Data)
		{
			TargetAttributeChangeSignature.Broadcast(Data.NewValue);
		}
	);
}

void AEnemyCharacter::ListenToHitReactTagChange()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(FBorisGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AEnemyCharacter::HitReactTagChanged
	);
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
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

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}
