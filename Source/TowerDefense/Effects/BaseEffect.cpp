// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "BaseEffect.h"

BaseEffect::BaseEffect(EElement element)
	: Element(element)
{

}

BaseEffect* BaseEffect::Clone()
{
	return new BaseEffect(Element);
}