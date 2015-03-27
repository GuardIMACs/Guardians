// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "StaticBehavior.h"
#include "BaseUnit.h"
#include "TowerDefenseGameMode.h"
#include "AIController.h"
#include "Buildings/Generator.h"

StaticBehavior::StaticBehavior() : UnitBehavior()
{
}

StaticBehavior::~StaticBehavior()
{
}

void StaticBehavior::Tick(ABaseUnit* unit, float elapsed)
{
	if (!unit)
		return;
	return;
// 	auto world = unit->GetWorld();
// 	if (world)
// 	{
// 		auto mode = world->GetAuthGameMode<ATowerDefenseGameMode>();
// 		if (mode)
// 		{
// 			auto generator = mode->Generator;
// 			auto a = unit->GetController();
// 			AAIController* ai = Cast<AAIController>(unit->GetController());
// 			if (ai && generator)
// 			{
// 				ai->MoveToActor(generator, 10.f);
// 				UE_LOG(LogTemp, Warning, TEXT("Moving to generator"));
// 			}
// 			
// 			if (!ai)
// 			{
// 				//UE_LOG(LogTemp, Warning, TEXT("No AI"));
// 			}
// 			if (!generator)
// 			{
// 				//UE_LOG(LogTemp, Warning, TEXT("No generator"));
// 			}
// 			//unit->MoveToActor();
// 		}
// 	}
	
	// Do nothing
}