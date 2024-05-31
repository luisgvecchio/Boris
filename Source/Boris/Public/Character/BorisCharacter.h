// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "BorisCharacter.generated.h"

class UInventoryBaseComponent;
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

	UFUNCTION(BlueprintCallable)
	UInventoryBaseComponent* GetInventoryComponent() { return InventoryComponent; }

	//FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	//TODO: Manage Action State// FORCEINLINE EActionState GetActionState() const { return ActionState; }

	void EquipWeapon(AWeaponBase* Weapon);
	void UnequipCurrentEquippedWeapon();

	void SendAbilitySpecHandleToEquippedWeapon(FGameplayEffectSpecHandle IncomingAbilitySpecHandle);

protected:	

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	//Inventory
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInventoryBaseComponent> InventoryComponent;

private:

	void InitAbilityActorInfo();	
};
