// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "StaticBehavior.h"
#include "BaseUnit.h"
#include "TowerDefenseGameMode.h"

StaticBehavior::StaticBehavior() : UnitBehavior()
{
}

StaticBehavior::~StaticBehavior()
{
}

void StaticBehavior::Tick(ABaseUnit* unit, float elapsed)
{
	if (!unit)
		return;

	auto world = unit->GetWorld();
	if (world)
	{
		auto mode = world->GetAuthGameMode<ATowerDefenseGameMode>();
		if (mode)
		{
			auto generator = mode->Generator;
			unit->MoveToActor();
		}
	}
	
	// Do nothing
}