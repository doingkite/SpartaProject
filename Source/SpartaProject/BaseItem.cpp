#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap); //Collision->OnComponentBeginOverlap
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

void ABaseItem::OnItemOverlap
(
	UPrimitiveComponent* OverlappedComp, //오버랩이 발생한 자기 자신 -> 스피어 컴포넌트(콜리전 컴포넌트)
	AActor* OtherActor, //아이템을 먹으러  간 플레이어
	UPrimitiveComponent* OtherComp, // 충돌 원인(플레이어의 캡슐 컴포넌트)
	int32 OtherBodyIndx,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if(OtherActor && (OtherActor->ActorHasTag("Player")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
		ActivateItem(OtherActor);
	}
}

void ABaseItem::OnItemEndOverlap
(
	UPrimitiveComponent* OverlappedComp, //오버랩이 발생한 자기 자신 -> 스피어 컴포넌트(콜리전 컴포넌트)
	AActor* OtherActor, //아이템을 먹으러  간 플레이어
	UPrimitiveComponent* OtherComp, // 충돌 원인(플레이어의 캡슐 컴포넌트)
	int32 OtherBodyIndx
)
{
	
}

void ABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle =nullptr;

	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer
		(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			2.0f,
			false
		);
	}
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();

}
