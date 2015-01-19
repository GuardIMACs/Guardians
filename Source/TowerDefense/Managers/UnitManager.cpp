#include "TowerDefense.h"
#include "UnitManager.h"
#include "Units/Tower.h"
#include "Units/Monster.h"


CUnitManager::CUnitManager()
{
	uint8 max = static_cast<uint8>(EMonster::Max);
	for (uint8 i = 0; i < max; ++i)
		MonstersInfo.Add({nullptr, -1,-1,-1});

	max = static_cast<uint8>(ETower::Max);
	for (uint8 i = 0; i < max; ++i)
		TowersInfo.Add({ nullptr, -1, -1, -1 });
}


CUnitManager::~CUnitManager()
{
}

void CUnitManager::registerMonster(AMonster* monster, EMonster monsterId, int32 cost, float buildTime, int32 weight)
{
	if (!monster)
		UE_LOG(LogTemp, Warning, TEXT("Nullptr added"));
	MonstersInfo[static_cast<uint8>(monsterId)] = SMonsterInfo(monster, cost, buildTime, weight);
}

void CUnitManager::registerTower(ATower* tower, ETower towerId, int32 cost, float buildTime, int32 weight)
{
	if (!tower)
		UE_LOG(LogTemp, Warning, TEXT("Nullptr added"));
	TowersInfo[static_cast<uint8>(towerId)] = STowerInfo(tower, cost, buildTime, weight);
}

void CUnitManager::registerLoot(ALoot* loot, EMonster monsterId, int32 probability)
{
	LootsInfo.Add({ loot, monsterId, probability });
}

AMonster* CUnitManager::instanciateMonster(EMonster id, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!MonstersInfo[static_cast<uint8>(id)].Monster)
		return nullptr;

	AMonster* ptr = MonstersInfo[static_cast<uint8>(id)].Monster->spawn(World, SpawnLocation, SpawnRotation);
	if (ptr)
		Monsters.Add(ptr);
	return ptr;
}

ATower* CUnitManager::instanciateTower(ETower id, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!TowersInfo[static_cast<uint8>(id)].Tower)
		return nullptr;

	ATower* ptr = TowersInfo[static_cast<uint8>(id)].Tower->spawn(World, SpawnLocation, SpawnRotation);
	if (ptr)
		Towers.Add(ptr);
	return ptr;
}

TArray<AMonster*>& CUnitManager::getMonstersAlive()
{
	return Monsters;
}

TArray<ATower*>& CUnitManager::getTowersAlive()
{
	return Towers;
}

void CUnitManager::removeMonster(AMonster* monster)
{
	Monsters.Remove(monster);
}

void CUnitManager::removeTower(ATower* tower)
{
	Towers.Remove(tower);
}