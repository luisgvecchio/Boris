// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/Data/ItemTypes.h"

#include "ItemBase.generated.h"

class USphereComponent;
class UItemTypes;

UCLASS()
class BORIS_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:
	AItemBase();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return ItemMesh; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UItemTypes* ItemType;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;	
};
