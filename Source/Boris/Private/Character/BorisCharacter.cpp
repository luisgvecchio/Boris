// Copyright Three Headed Monkey Studios


#include "Character/BorisCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/BorisPlayerState.h"

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
	BorisPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BorisPlayerState, this);
	AbilitySystemComponent = BorisPlayerState->GetAbilitySystemComponent();
	AttributeSet = BorisPlayerState->GetAttributeSet();
}