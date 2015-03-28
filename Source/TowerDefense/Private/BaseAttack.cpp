// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "BaseAttack.h"
#include "BaseUnit.h"
#include "TowerDefenseGameMode.h"
#include "Effects/BaseEffect.h"
#include "Sound/SoundCue.h"

#include "DrawDebugHelpers.h"

BaseAttack::~BaseAttack()
{
}

void BaseAttack::Tick(float elapsed)
{
	// Not ready to fire
	if (CurrentCooldown > 0)
		CurrentCooldown -= elapsed;
	else
	{
		if (Target != 0)
			if (!IsTargetInRange() || !GetTargetPtr())
				Target = 0;
			else
				Fire();
		else
			SearchTarget();
	}
}

void BaseAttack::Fire()
{
	CASSERT(Target, "No target set but fire() called");
	UE_LOG(LogTemp, Warning, TEXT("FIRE"));
	CurrentCooldown = Cooldown;

	auto* t = GetTargetPtr();
	if (!t)
		return;

	for (auto& e : EffectsApply)
		e->OnHit(t, getRandomFloat(MinDamages, MaxDamages));

	if (Sound && Parent->AudioComponent)
		UGameplayStatics::PlaySoundAttached(Sound, Parent->AudioComponent);
	//DrawDebugLine(Target->GetWorld(), Parent->GetActorLocation(), Target->GetActorLocation(), FColor::Blue, false, 1.f);
}

bool BaseAttack::IsTargetInRange()
{
	if (Target == 0 || !Parent)
		return false;

	auto* Tar = GetTargetPtr();
	if (!Tar)
		return false;

	// We get the positions and ignore height
	auto from = Parent->GetActorLocation();	from.Z = 0;
	auto to = Tar->GetActorLocation();	to.Z = 0;
	
	float dist = FVector::Dist(from, to);
	return dist >= RangeMin && dist <= RangeMax;
}

ABaseUnit* BaseAttack::GetTargetPtr()
{
	auto* mode = Parent->GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	if (!mode)
		return nullptr;
	return mode->Units.GetUnitById(Target);
}