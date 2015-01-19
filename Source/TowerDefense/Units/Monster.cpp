#include "TowerDefense.h"
#include "TowerDefenseGameMode.h"
#include "Monster.h"


AMonster::AMonster(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Type = EUnitType::Attacker;
}

void AMonster::OnDestroy()
{
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	mode->Units.removeMonster(this);
	UE_LOG(LogTemp, Warning, TEXT("%s died"), *Name);
}