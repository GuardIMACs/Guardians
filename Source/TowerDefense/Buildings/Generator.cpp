#include "TowerDefense.h"
#include "Generator.h"


AGenerator::AGenerator(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/generator.generator'"));
	GetMesh()->SetSkeletalMesh(mesh.Object);
	GetMesh()->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	GetMesh()->SetWorldLocation(FVector(0, -110, 0));

	GetCapsuleComponent()->SetCapsuleRadius(70);
	GetCapsuleComponent()->SetCapsuleHalfHeight(150);
}