// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Defines.h"

/**
 * 
 */

class ABaseUnit;
class BaseEffect;
class ATowerDefenseGameMode;

class TOWERDEFENSE_API BaseAttack
{
public:
	BaseAttack(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : Parent(parent), Target(0), GameMode(gameMode) {}

	virtual ~BaseAttack();

	ABaseUnit* GetTargetPtr();

	/** Name of the attack */
	FString Name;

	/** Description of the attack */
	FString Description;

	/** Minimal range of the attack (in meters) */
	float RangeMin;

	/** Maximal range of the attack (in meters) */
	float RangeMax;

	/** Time between each attack (in seconds) */
	float Cooldown;

	/** Current cooldown time */
	float CurrentCooldown;

	/** Min damages for each attack */
	float MinDamages;

	/** Max damages for each attack */
	float MaxDamages;

	/** */
	ABaseUnit* Parent;

	/** Primary target */
	uint32 Target;

	/** Effects applieds to the target */
	TArray<TSharedPtr<BaseEffect>> EffectsApply;

	/** Called every frame */
	virtual void Tick(float elapsed);

	/** Look for the closest nearby enemy */
	virtual void SearchTarget() {};

	/** Apply effects to a target */
	virtual void Fire();

	/** */
	virtual bool IsTargetInRange();

	ATowerDefenseGameMode* GameMode;
};