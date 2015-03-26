// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "TowerDefenseProjectile.generated.h"

UCLASS(config=Game)
class ATowerDefenseProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	TSubobjectPtr<class USphereComponent> CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	TSubobjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		TSubobjectPtr<class UParticleSystemComponent> ParticleComp;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

