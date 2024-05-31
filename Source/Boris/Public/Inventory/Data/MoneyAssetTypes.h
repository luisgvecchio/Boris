// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StaticMesh.h"
#include "MoneyAssetTypes.generated.h"

USTRUCT(BlueprintType)
struct FMoneytypesInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Amount = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh;
};

/**
 * 
 */
UCLASS()
class BORIS_API UMoneyAssetTypes : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMoneytypesInfo MoneyType;
};
