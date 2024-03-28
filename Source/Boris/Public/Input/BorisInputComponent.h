// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "BorisInputConfig.h"
#include "BorisInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class BORIS_API UBorisInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UBorisInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UBorisInputComponent::BindAbilityActions(const UBorisInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FBorisInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				//Action.InputTag is passed along as parameter to the callback PressedFunc, in case it takes an argunment of such type
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag); 
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
};
