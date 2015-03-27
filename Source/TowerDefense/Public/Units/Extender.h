#pragma once
#include "Monster.h"
#include "Extender.generated.h"

UCLASS()
class TOWERDEFENSE_API AExtender : public AMonster
{
public:
	GENERATED_UCLASS_BODY()

	AExtender* Spawn(UWorld* world, const FVector& vec, const FRotator rot);

};
