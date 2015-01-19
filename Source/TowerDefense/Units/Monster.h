#pragma once

#include "BaseUnit.h"
#include "Monster.generated.h"

UCLASS()
class TOWERDEFENSE_API AMonster : public ABaseUnit, public ISpawnable<AMonster>
{
public:
	GENERATED_UCLASS_BODY()

	virtual AMonster* spawn(UWorld* world, const FVector& vec, const FRotator rot) { return nullptr; };
	virtual void OnDestroy();
};

