
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

bool CMonsterWaves::isEmpty() {
	return (waves.Num() == 0); 
}

bool CMonsterWaves::HasNextMonster() {
	if (waves.Num() == 0)
		return false;

	return waves[0].Num() > 0;
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
	}
	return EMonster::Max;
}

void CMonsterWaves::NextWave()
{
	if (waves.Num() > 0)
		waves.RemoveAt(0);

	UE_LOG(LogTemp, Warning, TEXT("NEXT WAVE"));
}