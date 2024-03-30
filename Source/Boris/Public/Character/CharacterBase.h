// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "GameplayEffectTypes.h"
#include "CharacterBase.generated.h"

class UBorisAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UBoxComponent;

UCLASS()
class BORIS_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	void ActivateWeaponCollider();
	void DeactivateWeaponCollider();

	virtual UBorisAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UBoxComponent> WeaponBoxCollider;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;
	virtual int32 GetPlayerLevel() override;

	UPROPERTY()
	TObjectPtr<UBorisAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	void AddCharacterAbilities();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	void InitializePrimaryAttributes() const;

private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
