// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "BorisCharacter.generated.h"

class AWeaponBase;

/**
 * 
 */
UCLASS()
class BORIS_API ABorisCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:

	ABorisCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */


	//FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	//TODO: Manage Action State// FORCEINLINE EActionState GetActionState() const { return ActionState; }

	void EquipWeapon(AWeaponBase* Weapon);

	void SendAbilitySpecHandleToEquippedWeapon(FGameplayEffectSpecHandle IncomingAbilitySpecHandle);

protected:	

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

private:

	void InitAbilityActorInfo();	
};
