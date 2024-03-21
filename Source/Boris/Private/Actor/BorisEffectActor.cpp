// Copyright Three Headed Monkey Studios


#include "Actor/BorisEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/BorisAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
ABorisEffectActor::ABorisEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ABorisEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack! (Remove const_cast)
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UBorisAttributeSet* BorisAttributeSet = Cast<UBorisAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UBorisAttributeSet::StaticClass()));

		UBorisAttributeSet* MutableBorisAttributeSet = const_cast<UBorisAttributeSet*>(BorisAttributeSet);
		MutableBorisAttributeSet->SetHealth(BorisAttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void ABorisEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void ABorisEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABorisEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABorisEffectActor::EndOverlap);
}

