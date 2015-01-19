#pragma once

#include "BaseUnit.h"
#include "Tower.generated.h"

UCLASS()
class TOWERDEFENSE_API ATower : public ABaseUnit, public ISpawnable < ATower >
{
public:
	GENERATED_UCLASS_BODY()

	virtual ATower* spawn(UWorld* world, const FVector& vec, const FRotator rot) { return nullptr; };
	virtual void OnDestroy();
};

