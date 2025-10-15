#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IntermediateItemSpawnTable.generated.h"

USTRUCT(BlueprintType)
struct FIntermediateItemSpawnTable : public FTableRowBase
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
