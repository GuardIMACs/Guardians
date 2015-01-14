#include "TowerDefense.h"
#include "MonsterManager.h"
#include "Public/BaseUnit.h"
#include "Public/BaseAttack.h"
#include "Behaviors/StaticBehavior.h"

#include "Units/Gatling.h"
#include "Units/Voidling.h"

MonsterManager::MonsterManager()
{
	uint8 max = static_cast<uint8>(EMonster::Max);
	for (uint8 i = 0; i < max; ++i)
		Monsters.Add(nullptr);
}

MonsterManager::~MonsterManager()
{
}

void MonsterManager::Initialize(UWorld* world)
{
	//World = world;
	AddMonster(EMonster::Voidling, SpawnActor<AVoidling>(), 100, 1.f);
}

void MonsterManager::AddMonster(EMonster monster, ABaseUnit* clonable, int cost, float buildTime)
{
	if (!clonable)
		UE_LOG(LogTemp, Warning, TEXT("Nullptr added"));
	Monsters[static_cast<uint8>(monster)] = TSharedPtr<MonsterInfo>(new MonsterInfo(clonable, cost, buildTime));
}

ABaseUnit* MonsterManager::SpawnMonster(EMonster monster, UWorld* World, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	ABaseUnit* ptr = Monsters[static_cast<uint8>(monster)]->Unit->spawn(World, SpawnLocation, SpawnRotation);
	if (ptr)
		SpawnedMonsters.Add(ptr);
	return ptr;
}

TSharedPtr<MonsterInfo> MonsterManager::GetMonsterInfo(EMonster monster)
{
	return Monsters[static_cast<uint8>(monster)];
}