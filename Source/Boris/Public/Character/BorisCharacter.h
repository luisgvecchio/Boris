// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "BorisCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API ABorisCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	ABorisCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
private:
	void InitAbilityActorInfo();
};