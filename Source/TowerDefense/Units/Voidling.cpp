// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Voidling.h"
#include "Behaviors/StaticBehavior.h"
#include "TowerDefenseGameMode.h"

class VoidlingAtk : public BaseAttack
{
public:
	VoidlingAtk(ABaseUnit* parent, ATowerDefenseGameMode* gameMode) : BaseAttack(parent, gameMode)
	{
		Name = "Leap attack";
		Description = "Jumps on the closest target";
		RangeMin = 0.f;
		RangeMax = 5.f;
		Cooldown = 0.5;
		CurrentCooldown = 0.f;
		MinDamages = 10;
		MaxDamages = 20;
	}
};

AVoidling::AVoidling(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Name = "Gatling";
	Type = EUnitType::Attacker;
	MaxLife = 100;
	CurrentLife = MaxLife;
	Speed = 0.f;
	Behavior = TSharedPtr<UnitBehavior>(new StaticBehavior());

	/*if (GetWorld())
	{
		auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
		if (mode)
		{
			Attack.Add(TSharedPtr<BaseAttack>(new VoidlingAtk(this, mode)));
			UE_LOG(LogTemp, Warning, TEXT("created voidlingatk"));
		}
	}*/

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/gatling.gatling'"));
	Mesh->SetSkeletalMesh(mesh.Object);
	Mesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Mesh->SetWorldLocation(FVector(0, 0, 0));

	CapsuleComponent->SetCapsuleRadius(70);
	CapsuleComponent->SetCapsuleHalfHeight(150);
}

AVoidling* AVoidling::spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<AVoidling>(vec, rot);
}

void AVoidling::OnDestroy()
{
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	mode->Monsters.SpawnedMonsters.Remove(this);
	UE_LOG(LogTemp, Warning, TEXT("voidling died"));
}