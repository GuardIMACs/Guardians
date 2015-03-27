#include "TowerDefense.h"
#include "TowerDefenseGameMode.h"
#include "MonsterAIController.h"
#include "Monster.h"


AMonster::AMonster(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Type = EUnitType::Attacker;
	AIControllerClass = AMonsterAIController::StaticClass();
}

void AMonster::OnDestroy()
{
	auto* mode = GetWorld()->GetAuthGameMode<ATowerDefenseGameMode>();
	mode->Units.removeMonster(this);
	UE_LOG(LogTemp, Warning, TEXT("%s died"), *Name);
}