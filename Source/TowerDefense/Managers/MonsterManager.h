#pragma once

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Defines.h"

/**
*
*/
class ABaseUnit;

struct MonsterInfo
{
	MonsterInfo(ABaseUnit* u, int c, float t) : Unit(u), Cost(c), BuildTime(t) {}

	ABaseUnit* Unit;
	int Cost;
	float BuildTime; /** In seconds */
};

class TOWERDEFENSE_API MonsterManager
{
public:
	MonsterManager();
	~MonsterManager();

	void Initialize(UWorld* world);
	void Deinitialize();

	void AddMonster(EMonster monster, ABaseUnit* clonable, int cost, float buildTime);
	ABaseUnit* SpawnMonster(EMonster monster, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	TSharedPtr<MonsterInfo> GetMonsterInfo(EMonster monster);

	TArray<TSharedPtr<MonsterInfo>> Monsters;

	UWorld* World;
	TArray<ABaseUnit*> SpawnedMonsters;
};