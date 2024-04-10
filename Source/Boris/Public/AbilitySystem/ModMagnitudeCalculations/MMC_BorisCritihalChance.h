// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_BorisCritihalChance.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UMMC_BorisCriticalChance : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UMMC_BorisCriticalChance();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	float CalculateResult(float Wisdom, const int32& PlayerLevel) const;

private:

	FGameplayEffectAttributeCaptureDefinition WisdomDefinition;
};
