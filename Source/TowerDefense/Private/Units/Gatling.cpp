// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Units/Monster.h"
#include "Units/Gatling.h"
#include "Behaviors/StaticBehavior.h"
#include "Effects/StandardEffect.h"
#include "Sound/SoundCue.h"
#include "TowerDefenseGameMode.h"


class GatlingAtk : public BaseAttack
{
public:
	GatlingAtk(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : BaseAttack(parent, gameMode)
	{
		Name = "Basic attack";
		Description = "Make hole in their bodies!";
		RangeMin = 0.f;
		RangeMax = 500.f;
		Cooldown = 0.5;
		CurrentCooldown = 0.f;
		MinDamages = 10;
		MaxDamages = 20;
		EffectsApply.Add(TSharedPtr<BaseEffect>(new StandardEffect(EElement::Normal)));
		Sound = parent->AtkSound;
	}

	void SearchTarget()
	{
		float minDist = RangeMax + 1.f;
		auto from = Parent->GetActorLocation();	from.Z = 0;

		GameMode->Units.ForeachMonster([this, &minDist, from](AMonster* m) {
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

AGatling::AGatling(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Name = "Gatling";
	MaxLife = 100;
	CurrentLife = MaxLife;
	Behavior = TSharedPtr<UnitBehavior>(new StaticBehavior());
	static ConstructorHelpers::FObjectFinder<USoundCue> sound(TEXT("SoundCue'/Game/Audio/Tourelles/shoot-laserblast_Cue.shoot-laserblast_Cue'"));
	if (sound.Object)
		AtkSound = sound.Object;

	if (GetWorld())
	{
		auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
		if (mode)
		{
			Attack.Add(TSharedPtr<BaseAttack>(new GatlingAtk(this, mode)));
			//UE_LOG(LogTemp, Warning, TEXT("created gatlingatk"));
		}
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Turrets/GatlingTurret.GatlingTurret'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
	GetMesh()->SetWorldLocation(FVector(0, 0, -50));


	GetCapsuleComponent()->SetCapsuleRadius(120);
	GetCapsuleComponent()->SetCapsuleHalfHeight(150);
}

AGatling* AGatling::Spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	auto* a = world->SpawnActor<AGatling>(vec, rot);
	if (a)
		a->SetActorLocation(a->GetActorLocation() + FVector(0, 0, -100));
	return a;
}