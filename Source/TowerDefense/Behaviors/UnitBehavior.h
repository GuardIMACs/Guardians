// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TOWERDEFENSE_API UnitBehavior
{
public:
	UnitBehavior() = default;
	virtual ~UnitBehavior() {};

	virtual void Tick(float elapsed) = 0;
};
