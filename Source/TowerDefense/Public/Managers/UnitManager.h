#pragma once
#include "Defines.h"
#include <algorithm>

#include "Buildings/Generator.h"

class AMonster;
class ATower;
class ALoot;
class ATowerDefenseGameMode;

struct SMonsterInfo
{
	SMonsterInfo(AMonster* u, int32 c, float t, int32 w) : Monster(u), Cost(c), BuildTime(t), Weight(w) {}
	SMonsterInfo(const SMonsterInfo& other) : SMonsterInfo(other.Monster, other.Cost, other.BuildTime, other.Weight) {}

	AMonster* Monster;	/** Monster to clone */
	int32 Cost;			/** Monster build cost */
	float BuildTime;	/** Monster build time, in seconds */
	int32 Weight;		/** Monster map cost */
};

struct STowerInfo
{
	STowerInfo(ATower* u, int32 c, float t, int32 w) : Tower(u), Cost(c), BuildTime(t), Weight(w) {}
	STowerInfo(const STowerInfo& other) : STowerInfo(other.Tower, other.Cost, other.BuildTime, other.Weight) {}

	ATower* Tower;		/** Monster to clone */
	int32 Cost;			/** Monster build cost */
	float BuildTime;	/** Monster build time, in seconds */
	int32 Weight;		/** Monster map cost */
};

struct SLootInfo
{
	SLootInfo(ALoot* l, EMonster m, int32 p) : Loot(l), Monster(m), Probability(p) {}

	ALoot* Loot;
	EMonster Monster;
	int32 Probability;
};

class TOWERDEFENSE_API CUnitManager
{
public:
	CUnitManager();
	~CUnitManager();

	void registerMonster(AMonster* monster, EMonster monsterId, int32 cost, float buildTime, int32 weight = 1);
	void registerTower(ATower* tower, ETower towerId, int32 cost, float buildTime, int32 weight);
	void registerLoot(ALoot* loot, EMonster monsterId, int32 probability);

	AMonster*	instanciateMonster(EMonster id, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	ATower*		instanciateTower(ETower id, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	TArray<AMonster*>&	getMonstersAlive();
	TArray<ATower*>&	getTowersAlive();

	void		removeMonster(AMonster* monster);
	void		removeTower(ATower* tower);

	const SMonsterInfo& getMonsterInfo(EMonster monster) const;

	template<typename Lambda> void ForeachTower(Lambda l)
	{
		for (auto it = Towers.CreateIterator(); it; ++it)
			l(*it);
	}
	template<typename Lambda> void ForeachMonster(Lambda l)
	{
		for (auto it = Monsters.CreateIterator(); it; ++it)
			l(*it);
	}
	template<typename Lambda> void ForeachUnit(Lambda l)
	{
		for (auto it = Towers.CreateIterator(); it; ++it)
			l(*it);
		for (auto it = Monsters.CreateIterator(); it; ++it)
			l(*it);
		if (Generator)
			l(Generator);
	}

protected:
	TArray<SMonsterInfo>	MonstersInfo;
	TArray<AMonster*>		Monsters;

	TArray<STowerInfo>		TowersInfo;
	TArray<ATower*>			Towers;

	TArray<SLootInfo>		LootsInfo;
	TArray<ALoot*>			Loots;

public:
	AGenerator*				Generator;
	ATowerDefenseGameMode*	GameMode;
};

