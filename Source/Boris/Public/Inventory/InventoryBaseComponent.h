// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/Widgets/BorisUserWidget.h"

#include "InventoryBaseComponent.generated.h"

struct FItemTypeInfo;
class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BORIS_API UInventoryBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryBaseComponent();

	UFUNCTION(BlueprintCallable, Category = "Money")
	int GetMoneyamount() { return MoneyAmount; }
	UFUNCTION(BlueprintCallable, Category = "Money")
	void AddMoney(int MoneyToAdd) { MoneyAmount += MoneyToAdd; }

	UFUNCTION(BlueprintCallable, Category = "Items")
	int GetMaxInventorySize() { return MaxInventorySize; }

	UFUNCTION(BlueprintCallable, Category = "Items")
	void ResizeInventory(int NewSize);

	UFUNCTION(BlueprintCallable, Category = "Items")
	void AddItemToInventory(AItemBase* ItemToAdd);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UBorisUserWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")

	TSubclassOf<UBorisUserWidget> WBPClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<FItemTypeInfo> Items;

protected:

	virtual void BeginPlay() override;

		
private:

	UPROPERTY()
	int MoneyAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int MaxInventorySize = 8;
};
