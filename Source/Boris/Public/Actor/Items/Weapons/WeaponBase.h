// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Actor/Items/ItemBase.h"
#include "GameplayEffectTypes.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class ABorisCharacter;
class ACharacterBase;

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

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator, const FName& NewCollisionProfile);

	void DisableSphereCollision();

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	FORCEINLINE UBoxComponent* GetWeaponBoxCollider() { return WeaponBoxCollider; }

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageSpecHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UBoxComponent> WeaponBoxCollider;

	ACharacterBase* GetCharacterOwner() const { return CharacterOwner; }
	void SetCharacterOwner(AActor* TargetCharacterOwner);

	void ResetActorsToIgnore();
	TArray<AActor*> GetActorsToIgnore() const { return IgnoreActors; }

	void SetCollisiontypeFoWeaponboxCollider(const FName& NewCollisionProfile);

protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ApplyDamage(AActor* OverlapingActor);


	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TObjectPtr<ACharacterBase> CharacterOwner;

	TArray<AActor*> IgnoreActors;
private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
};
