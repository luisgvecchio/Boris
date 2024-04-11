// Copyright Three Headed Monkey Studios


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/BorisBlueprintFunctionLibrary.h"
#include "BorisAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BorisAttributeSet.h"
#include "BorisGameplayTags.h"

struct BorisDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	BorisDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, Armor, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, CriticalHitDamage, Source, false);
	}
};

static const BorisDamageStatics& DamageStatics()
{
	static BorisDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	//Critical
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	float Damage = Spec.GetSetByCallerMagnitude(FBorisGameplayTags::Get().Damage);

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(0.f, Armor);

	//Critical Calc. (2.f + SourceCriticalHitDamage * 0,01.f) Because SourceCriticalHitDamage is percentage.
	//Double damage + whatever extra percentage the CriticalHitDamage provides

	const bool bCriticalHit = FMath::RandRange(1, 100) <= SourceCriticalHitChance;

	if (bCriticalHit)
	{
		float AddedDamagePercent = SourceCriticalHitDamage * 0.01;
		float CriticalPercent = 2.f + AddedDamagePercent;

		UBorisAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

		Damage *= CriticalPercent;
	}

	//Minus armor
	Damage -= Armor;
	Damage = FMath::Max<float>(0.f, Damage);
	Damage = (int)round(Damage);

	const FGameplayModifierEvaluatedData EvaluatedData(UBorisAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}