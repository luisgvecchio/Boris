// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "GameplayEffectTypes.h"
#include "CharacterBase.generated.h"

class UAbilitySystemComponent;
class UBorisAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UCapsuleComponent;
class AWeaponBase;
class UAnimMontage;

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedWithWeapon UMETA(DisplayName = "Equipped with Weapon")
};

UCLASS()
class BORIS_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	//Weapon
	void ActivateWeaponCollider();
	void DeactivateWeaponCollider();
	void SendAbilitySpecHandleToEquippedWeapon(FGameplayEffectSpecHandle IncomingAbilitySpecHandle);
	
	//Getters
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UBorisAbilitySystemComponent* GetBorisAbilitySystemComponent() const;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }	
	virtual AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; }
	ECollisionChannel GetWeaponAttackOverlapChannel() const { return WeaponAttackOverlapChannel.GetValue(); }
	FORCEINLINE virtual ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE virtual ECharacterActionState GetActionState() const { return ActionState; }
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UCapsuleComponent* GetHitCollider_Implementation() const override { return HitCapsuleCollider; }
	//

	//Death
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	virtual void Die() override;

protected:

	//Combat
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	AWeaponBase* EquippedWeapon = nullptr;	

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TEnumAsByte<ECollisionChannel> WeaponAttackOverlapChannel;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponAttackCollisionProfile;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<UCapsuleComponent> HitCapsuleCollider;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName HitColliderCollisionProfile;
	//

	//ASC
	UPROPERTY()
	TObjectPtr<UBorisAbilitySystemComponent> AbilitySystemComponent;	

	//Abilities
	UPROPERTY(EditAnywhere, Category = "Abilities")

	TArray<TSubclassOf<UGameplayAbility>> CurrentAbilities;

	//Attributes
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;	

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	//FUNCTIONS
	virtual void BeginPlay() override;

	virtual FVector GetCombatSocketLocation_Implementation() override;
	virtual int32 GetPlayerLevel() override;

	void AddCharacterStartUpAbilities();
	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAdd);
	void RemoveAllCharacterAbilities();
	void RemoveGivenCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToRemove);


	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void SetCollisionTypeForHitCollider(const FName& NewCollisionProfile);

	//States

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::EAS_Unoccupied;

	//Dissolve Effects

	void DissolveMaterials();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
