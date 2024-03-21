// Copyright Three Headed Monkey Studios
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BorisPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class BORIS_API ABorisPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABorisPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
		UPROPERTY(EditAnywhere, Category = "Input");
		TObjectPtr<UInputMappingContext> BorisContext;	

		UPROPERTY(EditAnywhere, Category = "Input");
		TObjectPtr<UInputAction> MoveAction;

		void Move(const FInputActionValue& InputActionValue);

		void CursorTrace();

		IEnemyInterface* LastActor;
		IEnemyInterface* CurrentActor;
};
