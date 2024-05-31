// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemTypes.generated.h"

class UTexture2D;
class AItemBase;

USTRUCT(BlueprintType)
struct FItemTypeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Power = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AItemBase> Item;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Thumbnail;
};

/**
 * 
 */
UCLASS()
class BORIS_API UItemTypes : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemTypeInfo ItemInfo;
};
