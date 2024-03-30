// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"


class UWidgetComponent;
/**
 * 
 */
UCLASS()
class BORIS_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()		

public:
	AEnemyCharacter();

	virtual void BeginPlay() override;

	// Inherited via IEnemyInterface
	void HighlightActor() override;
	void UnHighlightActor() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
