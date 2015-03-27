#pragma once
#include "Monster.h"
#include "Sentinel.generated.h"

UCLASS()
class TOWERDEFENSE_API ASentinel : public AMonster
{
public:
	GENERATED_UCLASS_BODY()

		ASentinel* Spawn(UWorld* world, const FVector& vec, const FRotator rot);
};
