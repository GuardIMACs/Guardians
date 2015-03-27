// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Predator.h"
#include "Engine/Blueprint.h"
#include "Behaviors/StaticBehavior.h"
#include "Effects/StandardEffect.h"
#include "TowerDefenseGameMode.h"
#include "Units/Tower.h"

class PredatorAtk : public BaseAttack
{
public:
	PredatorAtk(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : BaseAttack(parent, gameMode)
	{
		Name = "Leap attack";
		Description = "Jumps on the closest target";
		RangeMin = 0.f;
		RangeMax = 120.f;
		Cooldown = 0.5;
		CurrentCooldown = 0.f;
		MinDamages = 0;
		MaxDamages = 10;
		EffectsApply.Add(TSharedPtr<BaseEffect>(new StandardEffect(EElement::Normal)));
	}

	void SearchTarget()
	{
		float minDist = RangeMax + 1.f;
		Target = nullptr;
		auto from = Parent->GetActorLocation();	from.Z = 0;

		GameMode->Units.ForeachTower([this, &minDist, from](ATower* m) {
			if (!m->IsAlive())
				return;
			auto pos = m->GetActorLocation();
			pos.Z = 0;
			float dist = FVector::Dist(from, pos);
			if (dist <= RangeMax && dist >= RangeMin)
			{
				if (dist < minDist)
				{
					Target = m;
					minDist = dist;
				}
			}
		});
	}
};

APredator::APredator(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Name = "Predator";
	Type = EUnitType::Attacker;
	MaxLife = 100;
	CurrentLife = MaxLife;
	Speed = 0.f;
	AIBehavior = MonsterAIBehavior::Run;
	Behavior = TSharedPtr<UnitBehavior>(new StaticBehavior());

	if (GetWorld())
	{
		auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
		if (mode)
		{
			Attack.Add(TSharedPtr<BaseAttack>(new PredatorAtk(this, mode)));
			//UE_LOG(LogTemp, Warning, TEXT("created voidlingatk"));
		}
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Aliens/Predator.Predator'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	GetMesh()->SetWorldLocation(FVector(0, 0, -25));
	GetMesh()->SetWorldRotation(FRotator(0, 90, 0));

	GetCapsuleComponent()->SetCapsuleRadius(30);
	GetCapsuleComponent()->SetCapsuleHalfHeight(60);

	static ConstructorHelpers::FObjectFinder<UObject> tree(TEXT("BehaviorTree'/Game/Blueprints/Monster_Behavior.Monster_Behavior'"));
	BehaviorTree = (UBehaviorTree*)tree.Object;
}

APredator* APredator::Spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<APredator>(vec, rot);
}