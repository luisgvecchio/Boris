// Copyright Three Headed Monkey Studios


#include "Character/BorisCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/BorisPlayerState.h"
#include "Player/BorisPlayerController.h"
#include "UI/HUD/BorisHUD.h"

ABorisCharacter::ABorisCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 450.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ABorisCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();

	AddCharacterAbilities();
}

void ABorisCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void ABorisCharacter::InitAbilityActorInfo()
{
	ABorisPlayerState* BorisPlayerState = GetPlayerState<ABorisPlayerState>();
	check(BorisPlayerState);
	AbilitySystemComponent = Cast<UBorisAbilitySystemComponent>(BorisPlayerState->GetAbilitySystemComponent());

	AbilitySystemComponent->InitActorInfo(BorisPlayerState, this);

	AttributeSet = BorisPlayerState->GetAttributeSet();

	if (ABorisPlayerController* BorisPlayerController = Cast<ABorisPlayerController>(GetController()))
	{
		if (ABorisHUD* BorisHUD = Cast<ABorisHUD>(BorisPlayerController->GetHUD()))
		{
			BorisHUD->InitOverlay(BorisPlayerController, BorisPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}