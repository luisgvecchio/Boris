// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_BorisCriticalHitDamage.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UMMC_BorisCriticalHitDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UMMC_BorisCriticalHitDamage();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	float CalculateResult(float Dexterity, const int32& PlayerLevel) const;

private:

	FGameplayEffectAttributeCaptureDefinition DexterityDefinition;
};
