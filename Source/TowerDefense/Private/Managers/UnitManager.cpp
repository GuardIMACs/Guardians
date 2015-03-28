#include "TowerDefense.h"
#include "UnitManager.h"
#include "Units/Tower.h"
#include "Units/Monster.h"
#include "TowerDefenseGameMode.h"


CUnitManager::CUnitManager()
{
	uint8 max = static_cast<uint8>(EMonster::Max);
	for (uint8 i = 0; i < max; ++i)
		MonstersInfo.Add({nullptr, -1,-1,-1, 0});

	max = static_cast<uint8>(ETower::Max);
	for (uint8 i = 0; i < max; ++i)
		TowersInfo.Add({ nullptr, -1, -1, -1 });
}


CUnitManager::~CUnitManager()
{
}

void CUnitManager::registerMonster(AMonster* monster, EMonster monsterId, int32 cost, float buildTime, int32 weight, uint32 loot)
{
	if (!monster)
		UE_LOG(LogTemp, Warning, TEXT("Nullptr added"));
	MonstersInfo[static_cast<uint8>(monsterId)] = SMonsterInfo(monster, cost, buildTime, weight, loot);
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
	uint8 index = static_cast<uint8>(id);
	if (!MonstersInfo[index].Monster)
		return nullptr;

	AMonster* ptr = MonstersInfo[index].Monster->Spawn(World, SpawnLocation, SpawnRotation);
	if (ptr) {
		Monsters.Add(ptr);
		ptr->SpawnDefaultController();
	}
	return ptr;
}

ATower* CUnitManager::instanciateTower(ETower id, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	uint8 index = static_cast<uint8>(id);
	if (!TowersInfo[index].Tower)
		return nullptr;

	ATower* ptr = TowersInfo[index].Tower->Spawn(World, SpawnLocation, SpawnRotation);
	if (ptr) {
		Towers.Add(ptr);
		ptr->SpawnDefaultController();
		ptr->GetCapsuleComponent()->SetCanEverAffectNavigation(true);
	}
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
	if (GameMode)
		GameMode->NotifyMonsterKilled(getMonsterInfo(monster->MonsterID).Loot);
}

void CUnitManager::removeTower(ATower* tower)
{
	Towers.Remove(tower);
	if (GameMode)
		GameMode->NotifyTowerDestroyed();
}

const SMonsterInfo& CUnitManager::getMonsterInfo(EMonster monster) const
{
	return MonstersInfo[static_cast<uint8>(monster)];
}

ABaseUnit* CUnitManager::GetUnitById(uint32 id)
{
	for (auto it = Towers.CreateIterator(); it; ++it)
		if ((*it)->UnitID == id)
			return *it;
	
	for (auto it = Monsters.CreateIterator(); it; ++it)
		if ((*it)->UnitID == id)
			return *it;

	if (Generator && Generator->UnitID == id)
		return Generator;
	return nullptr;
}