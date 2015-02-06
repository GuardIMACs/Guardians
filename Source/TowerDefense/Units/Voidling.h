#pragma once
#include "Monster.h"
#include "Voidling.generated.h"

UCLASS()
class TOWERDEFENSE_API AVoidling : public AMonster
{
	GENERATED_UCLASS_BODY()
public:
	AVoidling* Spawn(UWorld* world, const FVector& vec, const FRotator rot);
};
