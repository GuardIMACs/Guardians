#pragma once
#include "Monster.h"
#include "Predator.generated.h"

UCLASS()
class TOWERDEFENSE_API APredator : public AMonster
{
public:
	GENERATED_UCLASS_BODY()

		APredator* Spawn(UWorld* world, const FVector& vec, const FRotator rot);
};
