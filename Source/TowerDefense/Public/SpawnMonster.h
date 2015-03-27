// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Defines.h"
#include "SpawnMonster.generated.h"

UCLASS()
class TOWERDEFENSE_API ASpawnMonster : public AActor
{
		
public:	

	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleInstanceOnly, Category = Spawning)
	class UBoxComponent* WhereToSpawn;

	UFUNCTION(BlueprintPure, Category = Spawning)
	FVector GetRandomPointInVolume(); 
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	float SpawnTime; 

	EMonster NextMonster;
	
};
