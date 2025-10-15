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
		UPrimitiveComponent* OverlappedComp, //오버랩이 발생한 자기 자신 -> 스피어 컴포넌트(콜리전 컴포넌트)
		AActor* OtherActor, //아이템을 먹으러  간 플레이어
		UPrimitiveComponent* OtherComp, // 충돌 원인(플레이어의 캡슐 컴포넌트)
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

	//힐링 코인 - 즉시 발동 - 오버랩
	//지뢰 - 범위 내 오버랩 - 발동이 5초뒤 폭발 - 오버랩 - 데미지
};
