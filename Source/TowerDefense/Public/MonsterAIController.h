// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */

class ABaseUnit;

UCLASS()
class TOWERDEFENSE_API AMonsterAIController : public AAIController
{
	GENERATED_UCLASS_BODY()
	virtual void Possess(class APawn* pawn) override;

	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void SearchForEnemy();

	void SetEnemy(ABaseUnit* pawn);

	class UBehaviorTreeComponent* BehaviorComponent;
	class UBlackboardComponent* BlackboardComponent;
	
protected:
	uint8 KeyID_Enemy;
	uint8 KeyID_EnemyLocation;
};
