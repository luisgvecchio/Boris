// Copyright Three Headed Monkey Studios


#include "AbilitySystem/ModMagnitudeCalculations/MMC_MaxHealth.h"

#include "AbilitySystem/BorisAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDefinition.AttributeToCapture = UBorisAttributeSet::GetVitalityAttribute();
	VitalityDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude(VitalityDefinition, Spec, EvaluationParameters, Vitality);
	Vitality = FMath::Max<float>(Vitality, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return CalculateResult(Vitality, PlayerLevel);
}

float UMMC_MaxHealth::CalculateResult(float Vitality, const int32& PlayerLevel) const
{
	return 20.f + 2.f * Vitality + 5.f * PlayerLevel;
}
