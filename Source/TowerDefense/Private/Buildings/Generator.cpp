#include "TowerDefense.h"
#include "Buildings/Generator.h"
#include "TowerDefenseGameMode.h"

AGenerator::AGenerator(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	MaxLife = 500.f;
	CurrentLife = 500.f;
	Type = EUnitType::Defender;
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/generator.generator'"));
	Mesh->SetSkeletalMesh(mesh.Object);

	CapsuleComponent->SetCapsuleRadius(65);
	CapsuleComponent->SetCapsuleHalfHeight(70);
	CapsuleComponent->SetWorldScale3D(FVector(5, 5, 5));*/
}

void AGenerator::OnDestroy()
{
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	if (mode)
		mode->NotifyGeneratorDestroyed();
}

void AGenerator::TakeDamages(float damages, EElement element)
{
	ABaseUnit::TakeDamages(damages, element);
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	if (mode)
		mode->NotifyGeneratorUnderAttack();
}