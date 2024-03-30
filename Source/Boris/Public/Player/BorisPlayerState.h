// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "BorisPlayerState.generated.h"

class UBorisAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class BORIS_API ABorisPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABorisPlayerState();
	virtual UBorisAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBorisAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
