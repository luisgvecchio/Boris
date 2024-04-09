// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Actor/Items/ItemBase.h"
#include "GameplayEffectTypes.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class ABorisCharacter;

//TODO: Erase after implementing Damage properly
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class BORIS_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:

	AWeaponBase();

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void DisableSphereCollision();

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	FORCEINLINE UBoxComponent* GetWeaponBoxCollider() { return WeaponBoxCollider; }

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageSpecHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UBoxComponent> WeaponBoxCollider;

	ABorisCharacter* GetCharacterOwner() const { return CharacterOwner; }
	void SetCharacterOwner(AActor* TargetCharacterOwner);

	TArray<AActor*> IgnoreActors;		

protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ApplyDamage(AActor* OverlapingActor, TSubclassOf<UGameplayEffect> TargetDamageEffectClass);


	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TObjectPtr<ABorisCharacter> CharacterOwner;

	//TODO: Remove when applying damage properly
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
};
