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
class ISpawnable {
public:
	virtual ~ISpawnable() {}
	virtual T* Spawn(UWorld* world, const FVector& vec, const FRotator rot) = 0;
};

UCLASS()
class TOWERDEFENSE_API ABaseUnit : public ACharacter, public ISpawnable<ABaseUnit>
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
	UPROPERTY(BlueprintReadWrite, Category = "Unit Stats")
		int32 CurrentLife;

	/** Damage reduction of the unit. Each index is an EElement */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Stats")
		TArray<float> Defense;

	/** How many units are locking this unit for attack */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Unit Stats")
		uint32 Locks;

	/** Attacks of the unit */
	TArray<TSharedPtr<BaseAttack>> Attack;

	/** Unit behavior. Updated each frame */
	TSharedPtr<UnitBehavior> Behavior;

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnDestroy() {};

	virtual ABaseUnit* Spawn(UWorld* world, const FVector& vec, const FRotator rot) { return nullptr; }

	virtual void AddLock();
	virtual void RemoveLock();
	virtual bool IsAlive();

	void SetSpeed(float speed);
};