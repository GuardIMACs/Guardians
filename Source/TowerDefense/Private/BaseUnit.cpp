// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "BaseUnit.h"


ABaseUnit::ABaseUnit(const class FObjectInitializer& PCIP)
	: Super(PCIP), Type(EUnitType::Neutral), Locks(0)
{
	uint8 max = static_cast<uint8>(EElement::Max);
	Defense.Reserve(max);
	for (int i = 0; i < max; ++i)
		Defense.Add(0.5f);
}

void ABaseUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentLife <= 0)
	{
		// Some units still references this unit
		if (Locks > 0)
			return;

		OnDestroy();
		Destroy();
		return;
	}

	if (Behavior.IsValid())
		Behavior->Tick(this, DeltaSeconds);
	for (auto& atk : Attack)
		atk->Tick(DeltaSeconds);
}

void ABaseUnit::AddLock()
{
	Locks++;
}

void ABaseUnit::RemoveLock()
{
	Locks--;
}

bool ABaseUnit::IsAlive()
{
	return CurrentLife > 0;
}

void ABaseUnit::SetSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void ABaseUnit::TakeDamages(float damages, EElement element)
{
	CurrentLife -= damages*Defense[static_cast<int>(element)];
}