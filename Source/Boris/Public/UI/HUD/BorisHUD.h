// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <BorisHUD.generated.h>

class UBorisUserWidget;

class UAttributeSet;
class UBorisAbilitySystemComponent;
class UOverlayWidgetController;
class UBorisUserWidget;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class BORIS_API ABorisHUD : public AHUD
{
	GENERATED_BODY()

public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UBorisAbilitySystemComponent* ASC, UAttributeSet* AS);

	UPROPERTY()
	TObjectPtr<UBorisUserWidget>  OverlayWidget;

protected:

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBorisUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

};
