// Copyright Three Headed Monkey Studios

#include "Player/BorisPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BorisGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/BorisInputComponent.h"
#include "Interaction/EnemyInterface.h"

//TODO: Erese when: Find a better place for each ability to be activated or not depending on wheter the character has a weapon equipped etc.

#include "Character/BorisCharacter.h"


ABorisPlayerController::ABorisPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}



void ABorisPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void ABorisPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit)
		return;

	LastActor = CurrentActor;

	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != CurrentActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (CurrentActor) CurrentActor->HighlightActor();
	}
}

void ABorisPlayerController::AutoRun()
{
	if (!bAutoRunning) 
		return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ABorisPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FBorisGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
}

void ABorisPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FBorisGameplayTags::Get().InputTag_LMB) &&
		//TODO: Find a better place for each ability to be activated or not depending on wheter the character has a weapon equipped etc.
		Cast<ABorisCharacter>(GetASC()->GetAvatarActor())->GetCharacterState() == ECharacterState::ECS_EquippedWithWeapon)
	{
		if (GetASC()) 
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (GetASC() &&
		//TODO: Find a better place for each ability to be activated or not depending on wheter the character has a weapon equipped etc.
		Cast<ABorisCharacter>(GetASC()->GetAvatarActor())->GetCharacterState() == ECharacterState::ECS_EquippedWithWeapon)
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void ABorisPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FBorisGameplayTags::Get().InputTag_LMB) &&
		//TODO: Find a better place for each ability to be activated or not depending on wheter the character has a weapon equipped etc.
		Cast<ABorisCharacter>(GetASC()->GetAvatarActor())->GetCharacterState() == ECharacterState::ECS_EquippedWithWeapon)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if (bTargeting || bShiftKeyDown &&
		//TODO: Find a better place for each ability to be activated or not depending on wheter the character has a weapon equipped etc.
		Cast<ABorisCharacter>(GetASC()->GetAvatarActor())->GetCharacterState() == ECharacterState::ECS_EquippedWithWeapon)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UBorisAbilitySystemComponent* ABorisPlayerController::GetASC()
{
	if (BorisAbilitySystemComponent == nullptr)
	{
		BorisAbilitySystemComponent = Cast<UBorisAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BorisAbilitySystemComponent;
}

void ABorisPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(BorisContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(BorisContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ABorisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UBorisInputComponent* BorisInputComponent = CastChecked<UBorisInputComponent>(InputComponent);

	//BorisInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABorisPlayerController::Move);
	BorisInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	BorisInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ABorisPlayerController::ShiftPressed);
	BorisInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ABorisPlayerController::ShiftReleased);
}

void ABorisPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YarnRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YarnRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YarnRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}


}


