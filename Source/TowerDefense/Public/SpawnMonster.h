// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnMonster.generated.h"

UCLASS()
class TOWERDEFENSE_API ASpawnMonster : public AActor
{
		
public:	

	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleInstanceOnly, Category = Spawning)
	TSubobjectPtr<UBoxComponent> WhereToSPawn;

	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf<class AMonster> SpawnedMonster; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float SpawnDelayRangeLow; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float SpawnDelayRangeHigh; 

	UFUNCTION(BlueprintPure, Category = Spawning)
	FVector GetRandomPointInVolume(); 

	UPROPERTY(EditAnywhere, Category = Spawning)
	int32 nbMonsterMax ;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:

	float GetRandomSpawnDelay(); 

	float SpawnDelay; 

	void SpawnUnit(); 

	float SpawnTime; 

	int32 nbMonster;
	
	
	
};
