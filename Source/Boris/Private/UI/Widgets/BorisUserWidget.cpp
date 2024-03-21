// Copyright Three Headed Monkey Studios


#include "UI/Widgets/BorisUserWidget.h"

void UBorisUserWidget::SetWidgetController(UObject* TargetWidgetController)
{
	WidgetController = TargetWidgetController;
	OnWidgetControllerSet();
}
