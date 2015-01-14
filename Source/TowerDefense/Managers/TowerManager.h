// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Defines.h"

/**
 * 
 */
class ABaseUnit;

struct TowerInfo
{
	TowerInfo(ABaseUnit* u, int c, float t) : Unit(u), Cost(c), BuildTime(t) {}

	ABaseUnit* Unit;
	int Cost;
	float BuildTime; /** In seconds */
};

class TOWERDEFENSE_API TowerManager
{
public:
	TowerManager();
	~TowerManager();

	void Initialize(UWorld* world);
	void Deinitialize();

	void AddTower(ETower tower, ABaseUnit* clonable, int cost, float buildTime);
	ABaseUnit* SpawnTower(ETower tower, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	TSharedPtr<TowerInfo> GetTowerInfo(ETower tower);

	TArray<TSharedPtr<TowerInfo>> Towers;

	UWorld* World;
	TArray<ABaseUnit*> SpawnedTowers;
};
