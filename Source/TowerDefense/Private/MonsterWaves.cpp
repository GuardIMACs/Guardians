
#include "TowerDefense.h"
#include "MonsterWaves.h"


CMonsterWaves::CMonsterWaves()
{
}


CMonsterWaves::~CMonsterWaves()
{
}

void CMonsterWaves::SetWavesCount(uint8 count)
{
	waves.Empty();
	for (uint8 t = 0; t < count; ++t)
		waves.Add(TArray<EMonster>());
}

void CMonsterWaves::AddWaveElement(uint8 wave, EMonster monster)
{
	waves[wave].Add(monster);
}

EMonster CMonsterWaves::NextMonster()
{
	if (waves.Num() > 0)
	{
		if (waves[0].Num() > 0) {
			auto next = waves[0][0];
			waves[0].RemoveAt(0);
			return next;
		}
		waves.RemoveAt(0);
	}
	return EMonster::Max;
}