// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Monster.h"
#include "Gatling.h"
#include "Behaviors/StaticBehavior.h"
#include "Effects/StandardEffect.h"
#include "TowerDefenseGameMode.h"
#include "Widgets/LifeBar.h"

class GatlingAtk : public BaseAttack
{
public:
	GatlingAtk(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : BaseAttack(parent, gameMode)
	{
		Name = "Basic attack";
		Description = "Make hole in their bodies!";
		RangeMin = 0.f;
		RangeMax = 500.f;
		MinDamages = 10;
		MaxDamages = 20;

		Cooldown = 0.5;
		CurrentCooldown = 0.f;
		EffectsApply.Add(TSharedPtr<BaseEffect>(new StandardEffect(EElement::Normal)));
	}

	void SearchTarget()
	{
		auto& monsters = GameMode->Units.getMonstersAlive();
		TArray<ABaseUnit*> m;
		for (AMonster* u : monsters)
			m.Add(u);

		SearchFromArray(m);
	}
};

AGatling::AGatling(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Name = "Gatling";
	MaxLife = 100;
	CurrentLife = MaxLife;
	Speed = 0.f;
	SetMaxLife(MaxLife);
	SetCurrentLife(CurrentLife);
	Behavior = TSharedPtr<UnitBehavior>(new StaticBehavior());

	if (GetWorld())
	{
		auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
		if (mode)
		{
			Attack.Add(TSharedPtr<BaseAttack>(new GatlingAtk(this, mode)));
			UE_LOG(LogTemp, Warning, TEXT("created gatlingatk"));
		}
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/gatling.gatling'"));
	Mesh->SetSkeletalMesh(mesh.Object);
	Mesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Mesh->SetWorldLocation(FVector(0,-110,0));

	CapsuleComponent->SetCapsuleRadius(70);
	CapsuleComponent->SetCapsuleHalfHeight(150);
}

AGatling* AGatling::Spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<AGatling>(vec, rot);
}