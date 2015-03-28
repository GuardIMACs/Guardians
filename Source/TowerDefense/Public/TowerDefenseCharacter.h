// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "BaseUnit.h"
#include "TowerDefenseCharacter.generated.h"


UCLASS(config=Game)
class ATowerDefenseCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	virtual void PostInitializeComponents() override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* FirstPersonCameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATowerDefenseProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** AnimMontage to play each time we fire while aiming*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAimAnimation;

	/** AnimMontage to play each time we stop fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* IdleAnimation;

	/** AnimMontage to play each time we stop fire while aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* IdleAimAnimation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
	bool Is_Tower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
	bool Is_Monster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
	FString ViewedObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
	int32 ViewedObject_currentlife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
	int32 ViewedObject_maxlife;

	/** Character's health  - float from 0 to 1 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterStat)
	float P_health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStat)
	float P_shield; //generator

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float P_towersAvailable; //generator

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float P_resourcesAvailable; //generator

	/** Min damages inflicted by the weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Weapon_MinDamages;

	/** Max damages inflicted by the weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Weapon_MaxDamages;

	bool Weapon_IsFiring;

	bool Weapon_IsAiming;

	bool Weapon_WasFiring;

	/** Fire rate of the weapon, in bullets per seconds */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Weapon_FireRate;

	float Weapon_CurrentFireElapsedTime;

	/** impact effects */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<class AImpactEffect> ImpactTemplate;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	uint32 Resources;

	void AddResources(uint32 resources);

protected:
	/** name of bone/socket for muzzle in weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName MuzzleAttachPoint;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleFX;


protected:

	/** get targeting state */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool IsTargeting() const;

	/** get the modifier value for running speed */
	UFUNCTION(BlueprintCallable, Category = Pawn)
	float GetRunningSpeedModifier() const;

	/** get running state */
	UFUNCTION(BlueprintCallable, Category = Pawn)
	bool IsRunning() const;


	/** Handler for a touch input beginning. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Fires a projectile. */
	void OnFire();
	void OnStopFire();

	/** */
	void OnSecondFire();
	void OnStopSecondFire();

	void SpawnTurret();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	virtual void Tick(float DeltaSeconds) override;

	/** get weapon mesh (needs pawn owner to determine variant) */
	USkeletalMeshComponent* GetWeaponMesh() const;

	/** get the muzzle location of the weapon */
	FVector GetMuzzleLocation() const;

	/** spawn trail effect */
	void SpawnTrailEffect(const FVector& EndPoint);

	void SpawnMuzzleEffect();

	void SpawnImpactEffect(const FHitResult& Impact);

	void UpdateLookAtInfos();

	void FireWeapon();

	void StopFireWeapon();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
};

