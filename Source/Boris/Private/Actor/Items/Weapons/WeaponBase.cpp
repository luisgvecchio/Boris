// Copyright Three Headed Monkey Studios


#include "Actor/Items/Weapons/WeaponBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/BorisCharacter.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Inventory/InventoryBaseComponent.h"


AWeaponBase::AWeaponBase()
{
	WeaponBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Collider"));
	WeaponBoxCollider->SetupAttachment(GetRootComponent());
	WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);//SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeaponBase::SetCharacterOwner(AActor* TargetCharacterOwner)
{
	CharacterOwner = Cast<ACharacterBase>(TargetCharacterOwner);
}

void AWeaponBase::ResetActorsToIgnore()
{
	IgnoreActors.Reset();
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnBoxOverlap);
}

void AWeaponBase::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator, const FName& NewCollisionProfile)
{
	//ItemState = EItemState::EIS_Equipped;
	if (GetWorldTimerManager().IsTimerActive((SphereCollisionTimer)))
		GetWorldTimerManager().ClearTimer(SphereCollisionTimer);

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetCollisiontypeFoWeaponboxCollider(NewCollisionProfile);
	SetOwner(NewOwner);
	SetCharacterOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
}

void AWeaponBase::SetCollisiontypeFoWeaponboxCollider(const FName& NewCollisionProfile)
{
	if (WeaponBoxCollider)
	{
		WeaponBoxCollider->SetCollisionProfileName(NewCollisionProfile);
	}
}

void AWeaponBase::EnableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AWeaponBase::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponBase::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeaponBase::ApplyDamage(AActor* OverlapingActor)
{
	if (!HasAuthority())
		return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlapingActor))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
	}
}

void AWeaponBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABorisCharacter* Character = Cast<ABorisCharacter>(OtherActor);	

	if (Character && Character->GetCharacterState() == ECharacterState::ECS_EquippedWithWeapon)
	{
		Character->GetInventoryComponent()->AddItemToInventory(this);
	}
	else if (Character)
	{
		Character->GetInventoryComponent()->AddItemToInventory(this);
		Character->EquipWeapon(this);
	}
}

void AWeaponBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeaponBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IgnoreActors.AddUnique(this);
	if (IgnoreActors.Contains(OtherActor))
		return;

	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	//GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Yellow.WithAlpha(100), FString::Printf(TEXT("Triggered component: %s in the Actor: %s"), *OtherComp->GetName(), *OtherActor->GetName()));

	FVector BoxExtent = WeaponBoxCollider->GetScaledBoxExtent();

	FVector BoxHitSize = FVector(BoxExtent.X, BoxExtent.Y, BoxExtent.Z * 0.1f) * 1.1f;


	ECollisionChannel TargetChannel = OtherComp->GetCollisionObjectType();
	FHitResult BoxHit;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectQueryTypes;
	ObjectQueryTypes.AddUnique(UEngineTypes::ConvertToObjectType(TargetChannel));

	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Start,
		End,
		BoxHitSize * 1.1f,
		BoxTraceStart->GetComponentRotation(),
		ObjectQueryTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true);

	if (!BoxHit.GetActor())
		return;

	//GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Green.WithAlpha(100), FString::Printf(TEXT("Box trace; ComponentTriggered: %s, Point of contact:%s, Actor: %s"), *BoxHit.GetComponent()->GetName(), *BoxHit.ImpactPoint.ToString(), *BoxHit.GetActor()->GetName()));

	ApplyDamage(BoxHit.GetActor());

	IgnoreActors.AddUnique(BoxHit.GetActor());
}