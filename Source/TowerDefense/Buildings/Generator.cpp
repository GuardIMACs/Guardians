#include "TowerDefense.h"
#include "Generator.h"


AGenerator::AGenerator(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	MaxLife = 50.f;
	CurrentLife = 50.f;
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("SkeletalMesh'/Game/Meshes/generator.generator'"));
	Mesh->SetSkeletalMesh(mesh.Object);

	CapsuleComponent->SetCapsuleRadius(65);
	CapsuleComponent->SetCapsuleHalfHeight(70);
	CapsuleComponent->SetWorldScale3D(FVector(5, 5, 5));*/
}