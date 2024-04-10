// Copyright Three Headed Monkey Studios


#include "AbilitySystem/ModMagnitudeCalculations/MMC_BorisCritihalChance.h"

#include "AbilitySystem/BorisAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_BorisCriticalChance::UMMC_BorisCriticalChance()
{
	WisdomDefinition.AttributeToCapture = UBorisAttributeSet::GetVitalityAttribute();
	WisdomDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	RelevantAttributesToCapture.Add(WisdomDefinition);
}

float UMMC_BorisCriticalChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Wisdom = 0.f;
	GetCapturedAttributeMagnitude(WisdomDefinition, Spec, EvaluationParameters, Wisdom);
	Wisdom = FMath::Max<float>(Wisdom, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return CalculateResult(Wisdom, PlayerLevel);
}

float UMMC_BorisCriticalChance::CalculateResult(float Wisdom, const int32& PlayerLevel) const
{
	float result = 2.f * Wisdom + 0.2 * PlayerLevel;

	return (int)round(result);
}