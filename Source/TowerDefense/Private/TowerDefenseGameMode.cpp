// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TowerDefense.h"
#include "TowerDefenseGameMode.h"
#include "TowerDefenseHUD.h"
#include "TowerDefenseCharacter.h"
#include "Units/Gatling.h"
#include "Units/Voidling.h"
#include "Buildings/Generator.h"
#include "Defines.h"

#include "EngineUtils.h"

ATowerDefenseGameMode::ATowerDefenseGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP), Generator(nullptr)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATowerDefenseHUD::StaticClass();

	//Towers.Initialize(GetWorld());
	//Monsters.Initialize(nullptr);
	Units.registerTower(SpawnActor<AGatling>(), ETower::Gatling, 100, 1.f, 1);

	Units.registerMonster(SpawnActor<AVoidling>(), EMonster::Voidling, 100, 1.f, 1);

	
}
void ATowerDefenseGameMode::PostInitializeComponents()
{
	for (TActorIterator<AGenerator> itr(GetWorld()); itr; ++itr)
	{
		Generator = *itr;
	}
}
