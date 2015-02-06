// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "Managers/UnitManager.h"
#include "TowerDefenseGameMode.generated.h"

class AGenerator;

UCLASS(minimalapi)
class ATowerDefenseGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	void PostInitializeComponents() override;
	
	CUnitManager Units;
	AGenerator* Generator;
	//TowerManager Towers;
	//MonsterManager Monsters;
};



