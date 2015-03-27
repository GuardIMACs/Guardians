// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tower.h"
#include "Gatling.generated.h"
/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AGatling : public ATower
{
public:
	GENERATED_UCLASS_BODY()

	AGatling* Spawn(UWorld* world, const FVector& vec, const FRotator rot);
};