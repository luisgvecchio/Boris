// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_ArmorWarrior.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UMMC_ArmorWarrior : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_ArmorWarrior();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	float CalculateResult(float Vitality, const int32& PlayerLevel) const;

private:

	FGameplayEffectAttributeCaptureDefinition VitalityDefinition;
};
