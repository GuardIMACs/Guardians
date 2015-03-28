// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Sentinel.h"
#include "Engine/Blueprint.h"
#include "Behaviors/StaticBehavior.h"
#include "Effects/StandardEffect.h"
#include "TowerDefenseGameMode.h"
#include "Units/Tower.h"
#include "Animation/AnimBlueprint.h"


class SentinelAtk : public BaseAttack
{
public:
	SentinelAtk(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : BaseAttack(parent, gameMode)
	{
		Name = "Leap attack";
		Description = "Jumps on the closest target";
		RangeMin = 0.f;
		RangeMax = 240.f;
		Cooldown = 0.1;
		CurrentCooldown = 0.f;
		MinDamages = 0;
		MaxDamages = 5;
		EffectsApply.Add(TSharedPtr<BaseEffect>(new StandardEffect(EElement::Normal)));
	}

	void SearchTarget()
	{
		float minDist = RangeMax + 1.f;
		auto from = Parent->GetActorLocation();	from.Z = 0;

		GameMode->Units.ForeachUnit([this, &minDist, from](ABaseUnit* m) {
			if (m->Type != EUnitType::Defender)
				return;
			if (!m->IsAlive())
				return;
			auto pos = m->GetActorLocation();
			pos.Z = 0;
			float dist = FVector::Dist(from, pos);
			if (dist <= RangeMax && dist >= RangeMin)
			{
				if (dist < minDist)
				{
					Target = m->UnitID;
					minDist = dist;
				}
			}
		});
	}
};

ASentinel::ASentinel(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Name = "Sentinel";
	MonsterID = EMonster::Sentinel;
	Type = EUnitType::Attacker;
	MaxLife = 100;
	CurrentLife = MaxLife;
	AIBehavior = MonsterAIBehavior::Run;
	Behavior = TSharedPtr<UnitBehavior>(new StaticBehavior());

	if (GetWorld())
	{
		auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
		if (mode)
		{
			Attack.Add(TSharedPtr<BaseAttack>(new SentinelAtk(this, mode)));
			//UE_LOG(LogTemp, Warning, TEXT("created voidlingatk"));
		}
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Aliens/Sentinel/Sentinel.Sentinel'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetWorldScale3D(FVector(0.7f, 0.7f, 0.7f));
	GetMesh()->SetWorldLocation(FVector(0, 0, 0));
	GetMesh()->SetWorldRotation(FRotator(0, 90, 0));

	static ConstructorHelpers::FObjectFinder<UClass> anim(TEXT("Class'/Game/Meshes/Aliens/Sentinel/SentinelAnimBlueprint.SentinelAnimBlueprint_C'"));
	if (anim.Object)
		GetMesh()->SetAnimInstanceClass(anim.Object);

	GetCapsuleComponent()->SetCapsuleRadius(35);
	GetCapsuleComponent()->SetCapsuleHalfHeight(55);

	static ConstructorHelpers::FObjectFinder<UObject> tree(TEXT("BehaviorTree'/Game/Blueprints/Monster_Behavior.Monster_Behavior'"));
	BehaviorTree = (UBehaviorTree*)tree.Object;

	SetSpeed(100);
}

ASentinel* ASentinel::Spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<ASentinel>(vec, rot);
}