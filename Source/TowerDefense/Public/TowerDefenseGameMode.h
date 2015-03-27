// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "Managers/UnitManager.h"
#include "MonsterWaves.h"
#include "TowerDefenseGameMode.generated.h"

class AGenerator;
class ASpawnMonster;
class CMonsterWaves;

UCLASS(minimalapi)
class ATowerDefenseGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	void PostInitializeComponents() override;
	
	CUnitManager Units;
	AGenerator* Generator;
	TArray<ASpawnMonster*> Spawns;
	CMonsterWaves Waves;
	//TowerManager Towers;
	//MonsterManager Monsters;
};



