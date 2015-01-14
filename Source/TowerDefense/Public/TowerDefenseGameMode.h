// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "Managers/TowerManager.h"
#include "Managers/MonsterManager.h"
#include "TowerDefenseGameMode.generated.h"

UCLASS(minimalapi)
class ATowerDefenseGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	TowerManager Towers;
	MonsterManager Monsters;
};



