// Copyright Three Headed Monkey Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BorisEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class BORIS_API ABorisEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ABorisEffectActor();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
};
