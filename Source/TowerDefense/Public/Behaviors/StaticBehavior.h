// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Behaviors/UnitBehavior.h"

/**
 * 
 */
class TOWERDEFENSE_API StaticBehavior : public UnitBehavior
{
public:
	StaticBehavior();
	~StaticBehavior();

	void Tick(ABaseUnit* unit, float elapsed);
};
