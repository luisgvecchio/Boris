// Copyright Three Headed Monkey Studios


#include "AbilitySystem/ModMagnitudeCalculations/MMC_BorisCriticalHitDamage.h"

#include "AbilitySystem/BorisAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_BorisCriticalHitDamage::UMMC_BorisCriticalHitDamage()
{
	DexterityDefinition.AttributeToCapture = UBorisAttributeSet::GetVitalityAttribute();
	DexterityDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	RelevantAttributesToCapture.Add(DexterityDefinition);
}

float UMMC_BorisCriticalHitDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(DexterityDefinition, Spec, EvaluationParameters, Dexterity);
	Dexterity = FMath::Max<float>(Dexterity, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return CalculateResult(Dexterity, PlayerLevel);
}

float UMMC_BorisCriticalHitDamage::CalculateResult(float Dexterity, const int32& PlayerLevel) const
{
	float result = 4.f * Dexterity + 0.2 * PlayerLevel;

	return (int)round(result);
}