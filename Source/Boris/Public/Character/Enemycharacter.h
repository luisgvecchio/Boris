// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Data/CharacterClassInfo.h"
#include "EnemyCharacter.generated.h"


class UWidgetComponent;
class FOnAttributeChangedSignature;
class UBehaviorTree;
class ABorisAIController;
class AWeaponBase;

/**
 * 
 */
UCLASS()
class BORIS_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()		

public:
	AEnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;

	// Inherited via IEnemyInterface
	void HighlightActor() override;
	void UnHighlightActor() override;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/** end Combat Interface */

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

protected:

	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AWeaponBase> EnemyWeaponClass;

	UPROPERTY(VisibleDefaultsOnly)
	class UChildActorComponent* ChildActorWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABorisAIController> BorisAIController;

	void AddLambdaListenerToAttributeChange(FOnAttributeChangedSignature& TargetAttributeChangeSignature, FGameplayAttribute EventToListenTo);
	void ListenToHitReactTagChange();
};
