#pragma once
#include "Monster.h"
#include "GlobalHawk.generated.h"

UCLASS()
class TOWERDEFENSE_API AGlobalHawk : public AMonster
{
public:
	GENERATED_UCLASS_BODY()

		AGlobalHawk* Spawn(UWorld* world, const FVector& vec, const FRotator rot);
};
