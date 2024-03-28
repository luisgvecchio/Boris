// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BorisBlueprintFunctionLibrary.generated.h"

class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class BORIS_API UBorisAbilitysystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "BorisAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
};
