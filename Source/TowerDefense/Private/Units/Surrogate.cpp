// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Surrogate.h"
#include "Engine/Blueprint.h"
#include "Behaviors/StaticBehavior.h"
#include "Effects/StandardEffect.h"
#include "TowerDefenseGameMode.h"
#include "Units/Tower.h"
#include "Animation/AnimBlueprint.h"


class SurrogateAtk : public BaseAttack
{
public:
	SurrogateAtk(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : BaseAttack(parent, gameMode)
	{
		Name = "Leap attack";
		Description = "Jumps on the closest target";
		RangeMin = 0.f;
		RangeMax = 120.f;
		Cooldown = 0.5;
		CurrentCooldown = 0.f;
		MinDamages = 0;
		MaxDamages = 2;
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

ASurrogate::ASurrogate(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Name = "Surrogate";
	MonsterID = EMonster::Surrogate;
	Type = EUnitType::Attacker;
	MaxLife = 500;
	CurrentLife = MaxLife;
	AIBehavior = MonsterAIBehavior::Run;
	static ConstructorHelpers::FObjectFinder<USoundCue> sound(TEXT("SoundCue'/Game/Audio/Monstres/Surrogate/attack-surrogate_Cue.attack-surrogate_Cue'"));
	if (sound.Object)
		AtkSound = sound.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> sound2(TEXT("SoundCue'/Game/Audio/Monstres/Surrogate/dead-surrogate_Cue.dead-surrogate_Cue'"));
	if (sound2.Object)
		DeathSound = sound2.Object;

	Behavior = TSharedPtr<UnitBehavior>(new StaticBehavior());

	if (GetWorld())
	{
		auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
		if (mode)
		{
			Attack.Add(TSharedPtr<BaseAttack>(new SurrogateAtk(this, mode)));
			//UE_LOG(LogTemp, Warning, TEXT("created voidlingatk"));
		}
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Aliens/Surrogate/Surrogate.Surrogate'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	GetMesh()->SetWorldLocation(FVector(0, 0, 0));
	GetMesh()->SetWorldRotation(FRotator(0, 90, 0));

	static ConstructorHelpers::FObjectFinder<UClass> anim(TEXT("Class'/Game/Meshes/Aliens/Surrogate/SurrogateAnimBlueprint.SurrogateAnimBlueprint_C'"));
	if (anim.Object)
		GetMesh()->SetAnimInstanceClass(anim.Object);

	GetCapsuleComponent()->SetCapsuleRadius(45);
	GetCapsuleComponent()->SetCapsuleHalfHeight(80);

	static ConstructorHelpers::FObjectFinder<UObject> tree(TEXT("BehaviorTree'/Game/Blueprints/Monster_Behavior.Monster_Behavior'"));
	BehaviorTree = (UBehaviorTree*)tree.Object;

	SetSpeed(100);
}

ASurrogate* ASurrogate::Spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<ASurrogate>(vec, rot);
}