#pragma once
#include "Monster.h"
#include "Surrogate.generated.h"

UCLASS()
class TOWERDEFENSE_API ASurrogate : public AMonster
{
public:
	GENERATED_UCLASS_BODY()

		ASurrogate* Spawn(UWorld* world, const FVector& vec, const FRotator rot);
};
