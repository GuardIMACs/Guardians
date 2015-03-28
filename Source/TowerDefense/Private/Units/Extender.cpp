// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Extender.h"
#include "Engine/Blueprint.h"
#include "Behaviors/StaticBehavior.h"
#include "Effects/StandardEffect.h"
#include "TowerDefenseGameMode.h"
#include "Units/Tower.h"
#include "Animation/AnimBlueprint.h"

class ExtenderAtk : public BaseAttack
{
public:
	ExtenderAtk(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : BaseAttack(parent, gameMode)
	{
		Name = "Leap attack";
		Description = "Jumps on the closest target";
		RangeMin = 0.f;
		RangeMax = 120.f;
		Cooldown = 0.5;
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

AExtender::AExtender(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Name = "Extender";
	MonsterID = EMonster::Extender;
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
			Attack.Add(TSharedPtr<BaseAttack>(new ExtenderAtk(this, mode)));
			//UE_LOG(LogTemp, Warning, TEXT("created voidlingatk"));
		}
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Aliens/Extender/Extender.Extender'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
	GetMesh()->SetWorldLocation(FVector(0, 0, 0));
	GetMesh()->SetWorldRotation(FRotator(0, 90, 0));
	static ConstructorHelpers::FObjectFinder<UClass> anim(TEXT("Class'/Game/Meshes/Aliens/Extender/AliensAnimBlueprint.AliensAnimBlueprint_C'"));
	if (anim.Object)
		GetMesh()->SetAnimInstanceClass(anim.Object);

	GetCapsuleComponent()->SetCapsuleRadius(30);
	GetCapsuleComponent()->SetCapsuleHalfHeight(30);

	//UE_LOG(LogTemp, Warning, TEXT("MOVESPEED: %f"), GetCharacterMovement()->GetMaxSpeed());
	SetSpeed(100);
	static ConstructorHelpers::FObjectFinder<UObject> tree(TEXT("BehaviorTree'/Game/Blueprints/Monster_Behavior.Monster_Behavior'"));
	BehaviorTree = (UBehaviorTree*)tree.Object;


}

AExtender* AExtender::Spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<AExtender>(vec, rot);
}