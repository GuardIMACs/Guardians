#pragma once
#include "BaseUnit.h"
#include "Voidling.generated.h"

UCLASS()
class TOWERDEFENSE_API AVoidling : public ABaseUnit
{
public:
	GENERATED_UCLASS_BODY()

	AVoidling* spawn(UWorld* world, const FVector& vec, const FRotator rot);
	virtual void OnDestroy();
};
