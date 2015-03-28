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

	defeat = false; 

	//Towers.Initialize(GetWorld());
	//Monsters.Initialize(nullptr);
	Units.GameMode = this;
	Units.registerTower(SpawnActor<AGatling>(), ETower::Gatling, 100, 1.f, 1);
	Units.registerMonster(SpawnActor<AExtender>(), EMonster::Extender, 100, 1.f, 1, 10);
	Units.registerMonster(SpawnActor<APredator>(), EMonster::Predator, 100, 5.f, 1, 25);
	Units.registerMonster(SpawnActor<AGlobalHawk>(), EMonster::GlobalHawk, 100, 10.f, 1, 50);
	Units.registerMonster(SpawnActor<ASentinel>(), EMonster::Sentinel, 100, 10.f, 1, 75);
	Units.registerMonster(SpawnActor<ASurrogate>(), EMonster::Surrogate, 100, 10.f, 1, 100);

	Waves.SetWavesCount(6);
	Waves.AddWaveElement(1, EMonster::Extender);
	Waves.AddWaveElement(1, EMonster::Extender);
	Waves.AddWaveElement(1, EMonster::Extender);
	Waves.AddWaveElement(1, EMonster::Extender);
	Waves.AddWaveElement(1, EMonster::Extender);
	Waves.AddWaveElement(1, EMonster::Predator);
	Waves.AddWaveElement(1, EMonster::Extender);
	Waves.AddWaveElement(1, EMonster::Predator);
	Waves.AddWaveElement(1, EMonster::Extender);
	Waves.AddWaveElement(1, EMonster::Predator);
	Waves.AddWaveElement(1, EMonster::Predator);

	Waves.AddWaveElement(2, EMonster::Extender);
	Waves.AddWaveElement(2, EMonster::Extender);
	Waves.AddWaveElement(2, EMonster::Extender);
	Waves.AddWaveElement(2, EMonster::Predator);
	Waves.AddWaveElement(2, EMonster::Predator);
	Waves.AddWaveElement(2, EMonster::GlobalHawk);
	Waves.AddWaveElement(2, EMonster::Extender);
	Waves.AddWaveElement(2, EMonster::Predator);
	Waves.AddWaveElement(2, EMonster::Extender);
	Waves.AddWaveElement(2, EMonster::GlobalHawk);
	Waves.AddWaveElement(2, EMonster::Predator);

	Waves.AddWaveElement(3, EMonster::Extender);
	Waves.AddWaveElement(3, EMonster::Extender);
	Waves.AddWaveElement(3, EMonster::Extender);
	Waves.AddWaveElement(3, EMonster::Extender);
	Waves.AddWaveElement(3, EMonster::Extender);
	Waves.AddWaveElement(3, EMonster::GlobalHawk);
	Waves.AddWaveElement(3, EMonster::Extender);
	Waves.AddWaveElement(3, EMonster::GlobalHawk);
	Waves.AddWaveElement(3, EMonster::Extender);
	Waves.AddWaveElement(3, EMonster::Predator);
	Waves.AddWaveElement(3, EMonster::Sentinel);

	Waves.AddWaveElement(4, EMonster::Extender);
	Waves.AddWaveElement(4, EMonster::Extender);
	Waves.AddWaveElement(4, EMonster::Extender);
	Waves.AddWaveElement(4, EMonster::Extender);
	Waves.AddWaveElement(4, EMonster::GlobalHawk);
	Waves.AddWaveElement(4, EMonster::Predator);
	Waves.AddWaveElement(4, EMonster::GlobalHawk);
	Waves.AddWaveElement(4, EMonster::Sentinel);
	Waves.AddWaveElement(4, EMonster::Sentinel);
	Waves.AddWaveElement(4, EMonster::Sentinel);
	Waves.AddWaveElement(4, EMonster::Sentinel);

	Waves.AddWaveElement(5, EMonster::Extender);
	Waves.AddWaveElement(5, EMonster::Extender);
	Waves.AddWaveElement(5, EMonster::Predator);
	Waves.AddWaveElement(5, EMonster::GlobalHawk);
	Waves.AddWaveElement(5, EMonster::GlobalHawk);
	Waves.AddWaveElement(5, EMonster::Sentinel);
	Waves.AddWaveElement(5, EMonster::Sentinel);
	Waves.AddWaveElement(5, EMonster::Sentinel);
	Waves.AddWaveElement(5, EMonster::Sentinel);
	Waves.AddWaveElement(5, EMonster::Surrogate);
	Waves.AddWaveElement(5, EMonster::Surrogate);

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
		bool spawning = false;
		for (auto it = Spawns.CreateIterator(); it; ++it)
			if ((*it)->IsSpawning())
				spawning = true;

		UE_LOG(LogTemp, Warning, TEXT("Alive: %d"), Units.getMonstersAlive().Num());
		if (Units.getMonstersAlive().Num() == 0 && !Waves.HasNextMonster() && !spawning)
		{
			player->EndWave();
		}
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
	defeat = true; 
}