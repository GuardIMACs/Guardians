#pragma once
#include "Monster.h"
#include "Voidling.generated.h"

UCLASS()
class TOWERDEFENSE_API AVoidling : public AMonster
{
public:
	GENERATED_UCLASS_BODY()

	AVoidling* spawn(UWorld* world, const FVector& vec, const FRotator rot);
};
