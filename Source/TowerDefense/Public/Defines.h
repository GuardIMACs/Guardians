// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <assert.h>
#include <random>
#include "Defines.generated.h"

#define CASSERT(a,b) assert(a && !b);
#if 1
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
#endif
/**
 *
 */
UENUM(BlueprintType)
enum class EElement : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice"),
	Poison UMETA(DisplayName = "Poison"),

	Max
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Attacker UMETA(DisplayName = "Attacker"),
	Defender UMETA(DisplayName = "Defender"),
	Neutral UMETA(DisplayName = "Neutral"),
};

UENUM(BlueprintType)
enum class ETower : uint8
{
	Gatling UMETA(DisplayName = "Gatling"),
	Flamethrower UMETA(DisplayName = "Flamethrower"),

	Max
};

UENUM(BlueprintType)
enum class EMonster : uint8
{
	Voidling UMETA(DisplayName = "Voidling"),
	Max
};

static std::mt19937 generator;
inline float getRandomFloat(float min, float max) 
{ 
	std::uniform_real_distribution<float> dist(min, max);
	return dist(generator);
}

/** Spawns and returns class T, respects default rotation and translation of root component. */
template< class T >
T* SpawnActor()
{
	return T::StaticClass()->GetDefaultObject<T>();
}

/** Spawns and returns class T, respects default rotation and translation of root component. */
template< class T >
T* SpawnActor(UClass* Class, AActor* Owner = nullptr, APawn* Instigator = nullptr, bool bNoCollisionFail = false)
{
	return return (Class != nullptr) ? Cast<T>(Class->GetWorld()->SpawnActor(Class, NAME_None, nullptr, nullptr, nullptr, bNoCollisionFail, false, Owner, Instigator)) : nullptr;
}