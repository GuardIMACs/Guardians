// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "SpawnMonster.h"
#include "Managers/UnitManager.h"
#include "MonsterWaves.h"
#include "TowerDefenseGameMode.h"


// Sets default values
ASpawnMonster::ASpawnMonster(const class FObjectInitializer& PCIP)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WhereToSpawn = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WhereToSpawn"));

	RootComponent = WhereToSpawn;
	NextMonster = EMonster::Max;
}


FVector ASpawnMonster::GetRandomPointInVolume(){
	FVector RandomLocation;

	float MinX, MinY, MinZ; 
	float MaxX, MaxY, MaxZ; 

	FVector Origin;
	FVector BoxExtend;

	Origin = WhereToSpawn->Bounds.Origin;
	BoxExtend = WhereToSpawn->Bounds.BoxExtent;

	MinX = Origin.X - BoxExtend.X / 2.f; 
	MinY = Origin.Y - BoxExtend.Y / 2.f;
	MinZ = Origin.Z - BoxExtend.Z / 2.f;

	MaxX = Origin.X + BoxExtend.X / 2.f;
	MaxY = Origin.Y + BoxExtend.Y / 2.f;
	MaxZ = Origin.Z + BoxExtend.Z / 2.f;

	RandomLocation.X = FMath::FRandRange(MinX, MaxX); 
	RandomLocation.Y = FMath::FRandRange(MinY, MaxY);
	RandomLocation.Z = FMath::FRandRange(MinZ, MaxZ);

	return RandomLocation;
}

// Called every frame
void ASpawnMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	SpawnTime += DeltaTime; 
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	
	if (NextMonster == EMonster::Max)
		NextMonster = mode->Waves.NextMonster();
	if (NextMonster != EMonster::Max)
	{
		auto& infos = mode->Units.getMonsterInfo(NextMonster);
		if (SpawnTime > infos.BuildTime) {
			mode->Units.instanciateMonster(NextMonster, GetWorld(), GetRandomPointInVolume(), FRotator::ZeroRotator);
			SpawnTime = 0;
			NextMonster = EMonster::Max;
		}
	}
}

