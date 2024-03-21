// Copyright Three Headed Monkey Studios


#include "UI/WidgetController/BorisWidgetController.h"

void UBorisWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}
void UBorisWidgetController::BroadcastInitialValues()
{

}
void UBorisWidgetController::BindCallbacksToDependencies()
{

}