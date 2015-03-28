#pragma once

#include "Defines.h"

class TOWERDEFENSE_API CMonsterWaves
{
public:
	CMonsterWaves();
	~CMonsterWaves();

	void SetWavesCount(uint8 count);
	void AddWaveElement(uint8 wave, EMonster monster);
	EMonster NextMonster();
	bool isEmpty(); 

protected:
	TArray<TArray<EMonster>> waves;
};

