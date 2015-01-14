// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Behaviors/UnitBehavior.h"
#include "Defines.h"
#include "BaseAttack.h"
#include "BaseUnit.generated.h"

/**
 * 
 */

template <typename T>
class Spawnable {
public:
	virtual ~Spawnable() {}
	virtual T* spawn(UWorld* world, const FVector& vec, const FRotator rot) = 0;
};

UCLASS()
class TOWERDEFENSE_API ABaseUnit : public ACharacter, public Spawnable<ABaseUnit>
{
public:
	GENERATED_UCLASS_BODY()

	/** Name of the unit */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Info")
		FString Name;

	/** Attacker or defender */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Info")
		EUnitType Type;

	/** Maximum life of the unit */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Stats")
		int32 MaxLife;

	/** Current life of the unit */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Stats")
		int32 CurrentLife;

	/** Unit speed in m/s */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Stats")
		float Speed;

	/** Damage reduction of the unit. Each index is an EElement */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Stats")
		TArray<float> Defense;

	/** Attacks of the unit */
	TArray<TSharedPtr<BaseAttack>> Attack;

	/** Unit behavior. Updated each frame */
	TSharedPtr<UnitBehavior> Behavior;

	virtual ABaseUnit* spawn(UWorld* world, const FVector& vec, const FRotator rot);

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnDestroy();
};