// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "BaseAttack.h"
#include "BaseUnit.h"
#include "TowerDefenseGameMode.h"
#include "Effects/BaseEffect.h"

#include "DrawDebugHelpers.h"

void BaseAttack::Tick(float elapsed)
{
	// Not ready to fire
	if (CurrentCooldown > 0)
		CurrentCooldown -= elapsed;
	else
	{
		if (Target && (!IsTargetInRange() || !Target->IsAlive()))
		{
			if (Target)
				Target->RemoveLock();
			Target = nullptr;
		}
		if (Target)
			Fire();
		else
		{
			SearchTarget();
			if (Target)
				Target->AddLock();
		}
	}
}

void BaseAttack::Fire()
{
	CASSERT(Target, "No target set but fire() called");
	UE_LOG(LogTemp, Warning, TEXT("FIRE"));
	CurrentCooldown = Cooldown;
	for (auto& e : EffectsApply)
		e->OnHit(Target, getRandomFloat(MinDamages, MaxDamages));
	DrawDebugLine(Target->GetWorld(), Parent->GetActorLocation(), Target->GetActorLocation(), FColor::Blue, false, 1.f);
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