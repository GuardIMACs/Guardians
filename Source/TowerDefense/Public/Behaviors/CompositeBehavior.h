// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnitBehavior.h"

/**
 * 
 */

class TOWERDEFENSE_API CompositeBehavior : public UnitBehavior
{
public:
	CompositeBehavior();
	~CompositeBehavior();

	void Tick(ABaseUnit* unit, float elapsed);

	TArray<TSharedPtr<UnitBehavior>> Behaviors;
};
