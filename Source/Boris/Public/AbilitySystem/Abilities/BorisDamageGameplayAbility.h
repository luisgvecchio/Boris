// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BorisGameplayAbility.h"
#include "BorisDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisDamageGameplayAbility : public UBorisGameplayAbility
{
	GENERATED_BODY()	

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void UpdateDamageSpecHandle();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void FinishAtttack();
};
