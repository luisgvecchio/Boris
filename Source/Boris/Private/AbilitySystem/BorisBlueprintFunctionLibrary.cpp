// Copyright Three Headed Monkey Studios


#include "AbilitySystem/BorisBlueprintFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/BorisWidgetController.h"
#include "Player/BorisPlayerState.h"
#include "UI/HUD/BorisHUD.h"

UOverlayWidgetController* UBorisAbilitysystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ABorisHUD* BorisHUD = Cast<ABorisHUD>(PC->GetHUD()))
		{
			ABorisPlayerState* PS = PC->GetPlayerState<ABorisPlayerState>();
			UBorisAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return BorisHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}