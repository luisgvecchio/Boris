// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BorisAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UBorisAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
