#pragma once

#include "BaseUnit.h"
#include "Generator.generated.h"

UCLASS()
class TOWERDEFENSE_API AGenerator : public ABaseUnit
{
public:
	GENERATED_UCLASS_BODY()

		void OnDestroy();

	void TakeDamages(float damages, EElement element);
};

