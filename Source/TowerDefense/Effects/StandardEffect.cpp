// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "StandardEffect.h"
#include "BaseUnit.h"

StandardEffect::StandardEffect(EElement element)
	: BaseEffect(element)
{

}

StandardEffect* StandardEffect::Clone()
{
	return new StandardEffect(Element);
}

void StandardEffect::OnHit(ABaseUnit* target, float damages)
{
	target->CurrentLife -= damages*target->Defense[static_cast<int>(Element)];
//	UE_LOG();
}

void StandardEffect::Tick(float elapsed)
{
	return; // Standard attack, does nothing else than damages.
}