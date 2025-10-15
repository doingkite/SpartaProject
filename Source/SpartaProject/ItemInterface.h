#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class SPARTAPROJECT_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap
	(
		UPrimitiveComponent* OverlappedComp, //�������� �߻��� �ڱ� �ڽ� -> ���Ǿ� ������Ʈ(�ݸ��� ������Ʈ)
		AActor* OtherActor, //�������� ������  �� �÷��̾�
		UPrimitiveComponent* OtherComp, // �浹 ����(�÷��̾��� ĸ�� ������Ʈ)
		int32 OtherBodyIndx,
		bool bFromSweep,
		const FHitResult& SweepResult
	) = 0;

	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndx
	) = 0;

	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() const = 0;

	//���� ���� - ��� �ߵ� - ������
	//���� - ���� �� ������ - �ߵ��� 5�ʵ� ���� - ������ - ������
};
