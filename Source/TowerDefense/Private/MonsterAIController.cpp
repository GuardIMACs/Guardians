// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Buildings/Generator.h"
#include "Units/Monster.h"
#include "Units/Tower.h"
#include "TowerDefenseGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController(const class FObjectInitializer& PCIP) :
	Super(PCIP)
{
	BehaviorComponent = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree"));
	BlackboardComponent = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoard"));
}


void AMonsterAIController::Possess(class APawn* pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("POSSESS"));
	Super::Possess(pawn);

	AMonster* monster = Cast<AMonster>(pawn);
	if (monster && monster->BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("MONSTER & TREE"));
		BlackboardComponent->InitializeBlackboard(*monster->BehaviorTree->BlackboardAsset);
		KeyID_Enemy = BlackboardComponent->GetKeyID("Enemy");
		KeyID_EnemyLocation = BlackboardComponent->GetKeyID("Destination");
		BehaviorComponent->StartTree(*monster->BehaviorTree);
	}
}

void AMonsterAIController::SearchForEnemy()
{
	auto pawn = GetPawn();
	auto monster = Cast<AMonster>(pawn);

	if (!monster)
	{
		UE_LOG(LogTemp, Warning, TEXT("!monster"));
		return;
	}
	auto* mode = monster->GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	if (!mode)
	{
		UE_LOG(LogTemp, Warning, TEXT("!mode"));
		return;
	}

	float minDist = MAX_FLT;
	ABaseUnit* target = nullptr;
	switch (monster->AIBehavior) {
	case MonsterAIBehavior::Attack:
		{
			auto from = pawn->GetActorLocation();	from.Z = 0;

			mode->Units.ForeachTower([&minDist, &target, from](ATower* t) {
				auto pos = t->GetActorLocation();	pos.Z = 0;
				float dist = FVector::Dist(from, pos);
				if (dist <= minDist)
				{
					target = t;
					minDist = dist;
				}
			});
			if (!target && mode->Generator)
					target = mode->Generator;
		}
		break;
	case MonsterAIBehavior::Run:
		if (mode->Generator)
			target = mode->Generator;
	
		break;
	default:
		break;
	}
	if (target)
		SetEnemy(target);
	else
		UE_LOG(LogTemp, Warning, TEXT("!target"));
}

void AMonsterAIController::SetEnemy(ABaseUnit* pawn)
{
	BlackboardComponent->SetValueAsObject(KeyID_Enemy, pawn);
	BlackboardComponent->SetValueAsVector(KeyID_EnemyLocation, pawn->GetActorLocation());
}