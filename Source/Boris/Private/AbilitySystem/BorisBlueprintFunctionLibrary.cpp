// Copyright Three Headed Monkey Studios


#include "AbilitySystem/BorisBlueprintFunctionLibrary.h"

#include "BorisAbilityTypes.h"
#include "Game/BorisGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/BorisWidgetController.h"
#include "Player/BorisPlayerState.h"
#include "UI/HUD/BorisHUD.h"

UOverlayWidgetController* UBorisAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
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

void UBorisAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	ABorisGameModeBase* BorisGameMode = Cast<ABorisGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BorisGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = BorisGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

bool UBorisAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBorisGameplayEffectContext* BorisEffectContext = static_cast<const FBorisGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BorisEffectContext->IsBlockedHit();
	}
	return false;
}

bool UBorisAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBorisGameplayEffectContext* BorisEffectContext = static_cast<const FBorisGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BorisEffectContext->IsCriticalHit();
	}
	return false;
}

void UBorisAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FBorisGameplayEffectContext* BorisEffectContext = static_cast<FBorisGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BorisEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UBorisAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FBorisGameplayEffectContext* BorisEffectContext = static_cast<FBorisGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BorisEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}