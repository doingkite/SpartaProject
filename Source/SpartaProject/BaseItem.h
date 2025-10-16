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
		UPrimitiveComponent* OverlappedComp, //�������� �߻��� �ڱ� �ڽ� -> ���Ǿ� ������Ʈ(�ݸ��� ������Ʈ)
		AActor* OtherActor, //�������� ������  �� �÷��̾�
		UPrimitiveComponent* OtherComp, // �浹 ����(�÷��̾��� ĸ�� ������Ʈ)
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
