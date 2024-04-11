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

	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamageResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(RangedDamageResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	BorisDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, Armor, Target, false);		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, CriticalHitDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, PhysicalDamageResistance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBorisAttributeSet, RangedDamageResistance, Source, false);

		const FBorisGameplayTags& Tags = FBorisGameplayTags::Get();

		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);

		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical_Damage, PhysicalDamageResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Ranged_Damage, RangedDamageResistanceDef);

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

	RelevantAttributesToCapture.Add(DamageStatics().PhysicalDamageResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().RangedDamageResistanceDef);


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

	float Damage = 0.f;

	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FBorisGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(BorisDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = BorisDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;

		Damage += DamageTypeValue;
	}

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