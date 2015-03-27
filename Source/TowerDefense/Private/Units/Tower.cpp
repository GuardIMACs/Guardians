#include "TowerDefense.h"
#include "TowerDefenseGameMode.h"
#include "Tower.h"


ATower::ATower(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Type = EUnitType::Defender;
}

void ATower::OnDestroy()
{
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	mode->Units.removeTower(this);
	UE_LOG(LogTemp, Warning, TEXT("%s died"), *Name);
}