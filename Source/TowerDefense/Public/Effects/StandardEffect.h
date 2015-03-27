// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Effects/BaseEffect.h"

/**
 * 
 */

class TOWERDEFENSE_API StandardEffect : public BaseEffect
{
public:
	StandardEffect(EElement element);
	StandardEffect* Clone();

	void OnHit(ABaseUnit* target, float damages);
	void Tick(float elapsed);
};
