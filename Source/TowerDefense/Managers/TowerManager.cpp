// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "TowerManager.h"
#include "Public/BaseUnit.h"
#include "Public/BaseAttack.h"
#include "Behaviors/StaticBehavior.h"

#include "Units/Gatling.h"

TowerManager::TowerManager()
{
	uint8 max = static_cast<uint8>(ETower::Max);
	for (uint8 i = 0; i < max; ++i)
		Towers.Add(nullptr);
}

TowerManager::~TowerManager()
{
}

void TowerManager::Initialize(UWorld* world)
{
	//World = world;
	AddTower(ETower::Gatling, SpawnActor<AGatling>(), 100, 1.f);
}

void TowerManager::AddTower(ETower tower, ABaseUnit* clonable, int cost, float buildTime)
{
	if (!clonable)
		UE_LOG(LogTemp, Warning, TEXT("Nullptr added"));
	Towers[static_cast<uint8>(tower)] = TSharedPtr<TowerInfo>(new TowerInfo(clonable, cost, buildTime));
}

ABaseUnit* TowerManager::SpawnTower(ETower tower, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	ABaseUnit* ptr = Towers[static_cast<uint8>(tower)]->Unit->spawn(World, SpawnLocation, SpawnRotation);
	if (ptr)
		SpawnedTowers.Add(ptr);
	return ptr;
}

TSharedPtr<TowerInfo> TowerManager::GetTowerInfo(ETower tower)
{
	return Towers[static_cast<uint8>(tower)];
}