#include "TowerDefense.h"
#include "Generator.h"


AGenerator::AGenerator(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/generator.generator'"));
	Mesh->SetSkeletalMesh(mesh.Object);
	Mesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Mesh->SetWorldLocation(FVector(0, -110, 0));

	CapsuleComponent->SetCapsuleRadius(70);
	CapsuleComponent->SetCapsuleHalfHeight(150);
}