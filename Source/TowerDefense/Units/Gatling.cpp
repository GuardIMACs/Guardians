// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Gatling.h"
#include "Behaviors/StaticBehavior.h"
#include "Effects/StandardEffect.h"
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
	}

	void SearchTarget()
	{
		SearchFromArray(GameMode->Monsters.SpawnedMonsters);
	}
};

AGatling::AGatling(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Name = "Gatling";
	Type = EUnitType::Defender;
	MaxLife = 100;
	CurrentLife = MaxLife;
	Speed = 0.f;
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
	Mesh->SetWorldLocation(FVector(0,0,0));

	CapsuleComponent->SetCapsuleRadius(70);
	CapsuleComponent->SetCapsuleHalfHeight(150);
}

AGatling* AGatling::spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<AGatling>(vec, rot);
}

void AGatling::OnDestroy()
{
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	mode->Towers.SpawnedTowers.Remove(this);
}
