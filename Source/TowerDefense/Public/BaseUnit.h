// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Behaviors/UnitBehavior.h"
#include "Defines.h"
#include "BaseAttack.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "BaseUnit.generated.h"

class UBaseLifeBar;
/**
 * 
 */

template <typename T>
class ISpawnable {
public:
	virtual ~ISpawnable() {}
	virtual T* Spawn(UWorld* world, const FVector& vec, const FRotator rot) = 0;
};

class ILockable {
public:
	ILockable() : Locks(0) {}
	virtual ~ILockable() {}
	void AddLock() { Locks++; }
	void RemoveLock() { --Locks; }
	bool IsLocked() { return Locks > 0; }
private:
	int32_t Locks;
};

UCLASS()
class TOWERDEFENSE_API ABaseUnit : public ACharacter, public ISpawnable<ABaseUnit>, public ILockable
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

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnDestroy() {};

	virtual ABaseUnit* Spawn(UWorld* world, const FVector& vec, const FRotator rot) { return nullptr; }

	virtual void AddLock();
	virtual void RemoveLock();
	virtual bool IsAlive();

	void SetMaxLife(int32 life);
	void SetCurrentLife(int32 life);

	//UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "UI")
	//UWidgetComponent* Widget;
	
	//UBaseLifeBar* HealthBar;
};