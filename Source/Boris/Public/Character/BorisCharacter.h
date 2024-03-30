// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "BorisCharacter.generated.h"

//TODO: Erase after implementing Damage properly
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class BORIS_API ABorisCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	ABorisCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */

protected:

	//TODO: Remove when applying damage properly
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamage(AActor* OverlapingActor);

private:

	void InitAbilityActorInfo();
};
