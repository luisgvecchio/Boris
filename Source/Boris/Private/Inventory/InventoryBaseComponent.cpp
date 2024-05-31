// Copyright Three Headed Monkey Studios

#include "Inventory/InventoryBaseComponent.h"

#include "UI/Widgets/BorisUserWidget.h"
#include "Player/BorisPlayerController.h"
#include "Character/BorisCharacter.h"

#include "Actor/Items/ItemBase.h"
#include "Inventory/Data/ItemTypes.h"



// Sets default values for this component's properties
UInventoryBaseComponent::UInventoryBaseComponent()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Assets/blueprints/Inventory/WBP_Inventory"));

	if (ServerRowBPClass.Class)
	{
		WBPClass = ServerRowBPClass.Class;
	}
}

void UInventoryBaseComponent::ResizeInventory(int NewSize)
{
	Items.Reset(NewSize);
	MaxInventorySize = NewSize;
}

void UInventoryBaseComponent::AddItemToInventory(AItemBase* ItemToAdd)
{
	if (Items.Num() != MaxInventorySize)
	{
		Items.Add(ItemToAdd->ItemType->ItemInfo);
		ItemToAdd->Destroy();
	}
}

void UInventoryBaseComponent::BeginPlay()
{
	Super::BeginPlay();	

	//Set InventoryWidget Ref
	ABorisPlayerController* PlayerController = Cast<ABorisPlayerController>(Cast<ABorisCharacter>(GetOwner())->GetController());

	if (PlayerController)
	{
		InventoryWidget = Cast<UBorisUserWidget>(CreateWidget<UBorisUserWidget>(PlayerController, WBPClass, FName(TEXT("InventoryWidget"))));
	}

	Items.Reserve(MaxInventorySize);
}

