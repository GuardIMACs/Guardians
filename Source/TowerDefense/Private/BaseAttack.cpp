// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "BaseAttack.h"
#include "BaseUnit.h"
#include "TowerDefenseGameMode.h"
#include "Effects/BaseEffect.h"

void BaseAttack::Tick(float elapsed)
{
	// Not ready to fire
	if (CurrentCooldown > 0)
		CurrentCooldown -= elapsed;
	else
	{
		if (!IsTargetInRange())
			SearchTarget();
		else
			Fire();
	}
}

void BaseAttack::SearchFromArray(TArray<ABaseUnit*>& units)
{
	Target = nullptr;
	// Todo look for target between RangeMin et RangeMax

	float minRange = RangeMax + 1;
	ABaseUnit* target = nullptr;

	auto from = Parent->GetActorLocation();	from.Z = 0;


	for (auto* unit : units)
	{
		auto to = unit->GetActorLocation();	to.Z = 0;
		float dist = FVector::Dist(from, to);
		UE_LOG(LogTemp, Warning, TEXT("from: %f %f %f"), from.X, from.Y, from.Z);
		UE_LOG(LogTemp, Warning, TEXT("to: %f %f %f"), to.X, to.Y, to.Z);
		UE_LOG(LogTemp, Warning, TEXT("dist: %f"), dist);
		if (dist > RangeMin && dist < minRange)
		{
			minRange = dist;
			target = unit;
		}
	}

	if (target)
	{
		Target = target;
		UE_LOG(LogTemp, Warning, TEXT("Target Locked"));
	}
}

void BaseAttack::Fire()
{
	CASSERT(Target, "No target set but fire() called");
	UE_LOG(LogTemp, Warning, TEXT("FIRE"));
	CurrentCooldown = Cooldown;
	for (auto& e : EffectsApply)
		e->OnHit(Target, getRandomFloat(MinDamages, MaxDamages));
}

bool BaseAttack::IsTargetInRange()
{
	if (!Target || !Parent)
		return false;

	// We get the positions and ignore height
	auto from = Parent->GetActorLocation();	from.Z = 0;
	auto to = Target->GetActorLocation();	to.Z = 0;
	
	float dist = FVector::Dist(from, to);
	return dist >= RangeMin && dist <= RangeMax;
}