// Copyright Three Headed Monkey Studios


#include "BorisAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "BorisGameplayTags.h"

UBorisAssetManager& UBorisAssetManager::Get()
{
	check(GEngine);

	UBorisAssetManager* BorisAssetManager = Cast<UBorisAssetManager>(GEngine->AssetManager);
	return *BorisAssetManager;
}

void UBorisAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FBorisGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}

