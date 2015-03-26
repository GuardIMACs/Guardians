// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "SpawnMonster.h"
#include "Units/Monster.h"
#include "Units/Voidling.h"
#include "TowerDefenseGameMode.h"


// Sets default values
ASpawnMonster::ASpawnMonster(const class FPostConstructInitializeProperties& PCIP)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WhereToSPawn = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WhereToSpawn"));

	RootComponent = WhereToSPawn;

	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;
	SpawnDelay = GetRandomSpawnDelay(); 

	nbMonster = 0; 
}


void ASpawnMonster::SpawnUnit() {
	if (SpawnedMonster != NULL) {
		UWorld* const World = GetWorld(); 
		if (World) {
			FActorSpawnParameters SpawnParams; 
			SpawnParams.Owner = this; 
			SpawnParams.Instigator = Instigator; 


			FVector SpawnLocation = GetRandomPointInVolume(); 

			auto* mode = World->GetAuthGameMode<ATowerDefenseGameMode>();
			if (mode != nullptr)
			{
				mode->Units.instanciateMonster(EMonster::Voidling, World, SpawnLocation, FRotator::ZeroRotator);
			}

		}

	}

}


float ASpawnMonster::GetRandomSpawnDelay(){

	return FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);

}


FVector ASpawnMonster::GetRandomPointInVolume(){
	FVector RandomLocation;

	float MinX, MinY, MinZ; 
	float MaxX, MaxY, MaxZ; 

	FVector Origin;
	FVector BoxExtend;

	Origin = WhereToSPawn->Bounds.Origin;
	BoxExtend = WhereToSPawn->Bounds.BoxExtent; 

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




// Called when the game starts or when spawned
void ASpawnMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	SpawnTime += DeltaTime; 
	

	bool bShouldSpawn = (SpawnTime > SpawnDelay);

	if (bShouldSpawn) {

		nbMonster++; 
		SpawnUnit();

		SpawnTime -= SpawnDelay;
		SpawnDelay = GetRandomSpawnDelay();

		if (nbMonster > 10) {
			SpawnDelay += 30.f;
			nbMonster = 0; 

		}
	}

}

