#pragma once

#include "BaseUnit.h"
#include "Monster.generated.h"

UCLASS()
class TOWERDEFENSE_API AMonster : public ABaseUnit
{
public:
	GENERATED_UCLASS_BODY()

	virtual AMonster* Spawn(UWorld* world, const FVector& vec, const FRotator rot) { return nullptr; }
	virtual void OnDestroy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
	class UBehaviorTree* BehaviorTree;
	
};