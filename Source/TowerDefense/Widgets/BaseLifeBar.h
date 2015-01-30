#pragma once

#include "Defines.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "BaseLifeBar.generated.h"

UCLASS()
class TOWERDEFENSE_API UBaseLifeBar : public UUserWidget
{
public:
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintImplementableEvent, Category = "life")
		void SetMaxLife(int32 life);
	UFUNCTION(BlueprintImplementableEvent, Category = "life")
		void SetCurrentLife(int32 life);
	UFUNCTION(BlueprintImplementableEvent, Category = "life")
		void Update();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "life")
		int32 MaxLife;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "life")
		int32 CurrentLife;

};

