// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Voidling.h"
#include "Engine/Blueprint.h"
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
	Name = "Voidling";
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/Aliens/Extender.Extender'"));
	Mesh->SetSkeletalMesh(mesh.Object);
	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Mesh->SetWorldLocation(FVector(0, 0, 0));
	Mesh->SetWorldRotation(FRotator(0, 90, 0));

	CapsuleComponent->SetCapsuleRadius(30);
	CapsuleComponent->SetCapsuleHalfHeight(60);

	static ConstructorHelpers::FObjectFinder<UObject> tree(TEXT("BehaviorTree'/Game/Blueprints/Monster_Behavior.Monster_Behavior'"));
	BehaviorTree = (UBehaviorTree*)tree.Object;
}

AVoidling* AVoidling::Spawn(UWorld* world, const FVector& vec, const FRotator rot)
{
	return world->SpawnActor<AVoidling>(vec, rot);
}