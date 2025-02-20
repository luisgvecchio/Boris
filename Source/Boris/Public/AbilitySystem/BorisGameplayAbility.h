// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BorisGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool CheckIfActive();
};
