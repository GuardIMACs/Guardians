// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TowerDefense.h"
#include "TowerDefenseGameMode.h"
#include "TowerDefenseHUD.h"
#include "TowerDefenseCharacter.h"
#include "Units/Gatling.h"
#include "Units/Extender.h"
#include "Units/Predator.h"
#include "Units/GlobalHawk.h"
#include "Units/Sentinel.h"
#include "Units/Surrogate.h"
#include "Buildings/Generator.h"
#include "SpawnMonster.h"
#include "Defines.h"

#include "EngineUtils.h"

ATowerDefenseGameMode::ATowerDefenseGameMode(const class FObjectInitializer& PCIP)
	: Super(PCIP), Generator(nullptr)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATowerDefenseHUD::StaticClass();

	//Towers.Initialize(GetWorld());
	//Monsters.Initialize(nullptr);
	Units.GameMode = this;
	Units.registerTower(SpawnActor<AGatling>(), ETower::Gatling, 100, 1.f, 1);
	Units.registerMonster(SpawnActor<AExtender>(), EMonster::Extender, 100, 1.f, 1, 10);
	Units.registerMonster(SpawnActor<APredator>(), EMonster::Predator, 100, 5.f, 1, 25);
	Units.registerMonster(SpawnActor<AGlobalHawk>(), EMonster::GlobalHawk, 100, 10.f, 1, 50);
	Units.registerMonster(SpawnActor<ASentinel>(), EMonster::Sentinel, 100, 15.f, 1, 75);
	Units.registerMonster(SpawnActor<ASurrogate>(), EMonster::Surrogate, 100, 30.f, 1, 100);

	Waves.SetWavesCount(1);
	Waves.AddWaveElement(0, EMonster::Extender);
	Waves.AddWaveElement(0, EMonster::Extender);
	Waves.AddWaveElement(0, EMonster::Extender);
	Waves.AddWaveElement(0, EMonster::Extender);
	Waves.AddWaveElement(0, EMonster::Extender);
	Waves.AddWaveElement(0, EMonster::Predator);
	Waves.AddWaveElement(0, EMonster::Extender);
	Waves.AddWaveElement(0, EMonster::Predator);
	Waves.AddWaveElement(0, EMonster::Extender);
	Waves.AddWaveElement(0, EMonster::Predator);
	Waves.AddWaveElement(0, EMonster::Predator);

}
void ATowerDefenseGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	for (TActorIterator<AGenerator> itr(GetWorld()); itr; ++itr)
	{
		Generator = *itr;
		Units.Generator = Generator;
		UE_LOG(LogTemp, Warning, TEXT("Generator found"));
	}

	for (TActorIterator<ASpawnMonster> itr(GetWorld()); itr; ++itr)
		Spawns.Add(*itr);
}

void ATowerDefenseGameMode::NotifyMonsterKilled(uint32 resources)
{
	auto* player = Cast<ATowerDefenseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player)
	{
		player->AddResources(resources);
	}
}

void ATowerDefenseGameMode::NotifyTowerUnderAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Tower under attack!"));
}

void ATowerDefenseGameMode::NotifyGeneratorUnderAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Generator under attack!"));
}

void ATowerDefenseGameMode::NotifyTowerDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("Tower destroyed!"));
}

void ATowerDefenseGameMode::NotifyGeneratorDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("generator destroyed!"));
}