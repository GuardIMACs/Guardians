// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Public/BaseUnit.h"
#include "Gatling.generated.h"
/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AGatling : public ABaseUnit
{
public:
	GENERATED_UCLASS_BODY()

	AGatling* spawn(UWorld* world, const FVector& vec, const FRotator rot);
	virtual void OnDestroy();
};