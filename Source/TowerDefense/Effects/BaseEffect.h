// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Public/Defines.h"

/**
 * 
 */

class ABaseUnit;

class TOWERDEFENSE_API BaseEffect
{
public:
	BaseEffect(EElement element);
	virtual ~BaseEffect() {};

	BaseEffect* Clone();
	
	/** Function triggered when the attack hits the target */
	virtual void OnHit(ABaseUnit* target, float damages) {};

	/** Update function */
	virtual void Tick(float elapsed) {};

	/** Effect primary element */
	EElement Element;
};
