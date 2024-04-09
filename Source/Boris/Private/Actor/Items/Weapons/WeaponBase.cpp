// Copyright Three Headed Monkey Studios


#include "Actor/Items/Weapons/WeaponBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Character/BorisCharacter.h"
#include "AbilitySystem/BorisAbilitySystemComponent.h"
#include "BorisGameplayTags.h"

//TODO: Erase after implementing Damage properly
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/EnemyCharacter.h"

AWeaponBase::AWeaponBase()
{
	WeaponBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box Collider"));
	WeaponBoxCollider->SetupAttachment(GetRootComponent());
	WeaponBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeaponBase::SetCharacterOwner(AActor* TargetCharacterOwner)
{
	CharacterOwner = Cast<ABorisCharacter>(TargetCharacterOwner);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnBoxOverlap);

}

void AWeaponBase::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	//ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetCharacterOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
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

void AWeaponBase::ApplyDamage(AActor* OverlapingActor, TSubclassOf<UGameplayEffect> TargetDamageEffectClass)
{
	UBorisAbilitySystemComponent* ABS = GetCharacterOwner()->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = ABS->MakeOutgoingSpec(TargetDamageEffectClass, 0, ABS->MakeEffectContext());

	FBorisGameplayTags GameplayTags = FBorisGameplayTags::Get();
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 50.f);

	if (!HasAuthority())
		return;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlapingActor))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AWeaponBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABorisCharacter* Character = Cast<ABorisCharacter>(OtherActor);

	if(Character)
		Character->EquipWeapon(this);
}

void AWeaponBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AWeaponBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AEnemyCharacter>(OtherActor))
		return;

	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true);

	ApplyDamage(OtherActor, DamageEffectClass);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	if (BoxHit.GetActor())
	{
		IgnoreActors.AddUnique(BoxHit.GetActor());
	}
}