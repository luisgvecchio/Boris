// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Enemycharacter.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API AEnemycharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()		

public:
	AEnemycharacter();

	virtual void BeginPlay() override;

	// Inherited via IEnemyInterface
	void HighlightActor() override;
	void UnHighlightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

};
