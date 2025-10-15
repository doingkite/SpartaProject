#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AdvancedItemSpawnTable.generated.h"

USTRUCT(BlueprintType)
struct FAdvancedItemSpawnTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
