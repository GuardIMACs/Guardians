#pragma once

#include "BaseUnit.h"
#include "Tower.generated.h"

UCLASS()
class TOWERDEFENSE_API ATower : public ABaseUnit
{
public:
	GENERATED_UCLASS_BODY()

	virtual ATower* Spawn(UWorld* world, const FVector& vec, const FRotator rot) { return nullptr; }
	virtual void OnDestroy();
	virtual void TakeDamages(float damages, EElement element);
};

