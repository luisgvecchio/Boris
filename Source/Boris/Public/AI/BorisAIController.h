// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BorisAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class BORIS_API ABorisAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABorisAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
