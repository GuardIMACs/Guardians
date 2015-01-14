// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "BaseUnit.h"


ABaseUnit::ABaseUnit(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP), Type(EUnitType::Neutral)
{
	uint8 max = static_cast<uint8>(EElement::Max);
	Defense.Reserve(max);
	for (int i = 0; i < max; ++i)
		Defense.Add(0.5f);
}

ABaseUnit* ABaseUnit::spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return nullptr;
}

void ABaseUnit::Tick(float DeltaSeconds)
{
	if (CurrentLife <= 0)
	{
		OnDestroy();
		Destroy();
		return;
	}
	if (Behavior.IsValid())
		Behavior->Tick(DeltaSeconds);
	for (auto& atk : Attack)
		atk->Tick(DeltaSeconds);
}

void ABaseUnit::OnDestroy()
{

}