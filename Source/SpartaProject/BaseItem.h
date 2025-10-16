#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPROJECT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	
	ABaseItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Item|Effects")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Item|Effects")
	USoundBase* PickupSound;

	virtual void OnItemOverlap
	(
		UPrimitiveComponent* OverlappedComp, //오버랩이 발생한 자기 자신 -> 스피어 컴포넌트(콜리전 컴포넌트)
		AActor* OtherActor, //아이템을 먹으러  간 플레이어
		UPrimitiveComponent* OtherComp, // 충돌 원인(플레이어의 캡슐 컴포넌트)
		int32 OtherBodyIndx,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

	virtual void OnItemEndOverlap
	(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndx
	) override;

	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	virtual void DestroyItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
};
