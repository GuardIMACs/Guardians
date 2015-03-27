// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#include "TowerDefense.h"
#include "FX/ImpactEffect.h"
#include "Sound/SoundCue.h"

AImpactEffect::AImpactEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutoDestroyWhenFinished = true;
}

void AImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial* HitPhysMat = SurfaceHit.PhysMaterial.Get();
	//EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	// show particles
	UParticleSystem* ImpactFX = GetImpactFX(EPhysicalSurface::SurfaceType1);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	// play sound
	USoundCue* ImpactSound = GetImpactSound(EPhysicalSurface::SurfaceType1);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (DefaultDecal.DecalMaterial)
	{
		FRotator RandomDecalRotation = SurfaceHit.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached(DefaultDecal.DecalMaterial, FVector(DefaultDecal.DecalSize, DefaultDecal.DecalSize, 1.0f),
			SurfaceHit.Component.Get(), SurfaceHit.BoneName,
			SurfaceHit.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition,
			DefaultDecal.LifeSpan);
	}
}

UParticleSystem* AImpactEffect::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem* ImpactFX = NULL;
	ImpactFX = DefaultFX;
	/*switch (SurfaceType)
	{
	case SHOOTER_SURFACE_Concrete:	ImpactFX = ConcreteFX; break;
	case SHOOTER_SURFACE_Dirt:		ImpactFX = DirtFX; break;
	case SHOOTER_SURFACE_Water:		ImpactFX = WaterFX; break;
	case SHOOTER_SURFACE_Metal:		ImpactFX = MetalFX; break;
	case SHOOTER_SURFACE_Wood:		ImpactFX = WoodFX; break;
	case SHOOTER_SURFACE_Grass:		ImpactFX = GrassFX; break;
	case SHOOTER_SURFACE_Glass:		ImpactFX = GlassFX; break;
	case SHOOTER_SURFACE_Flesh:		ImpactFX = FleshFX; break;
	default:						ImpactFX = DefaultFX; break;
	}*/

	return ImpactFX;
}

USoundCue* AImpactEffect::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	USoundCue* ImpactSound = NULL;
	ImpactSound = DefaultSound;
	/*switch (SurfaceType)
	{
	case SHOOTER_SURFACE_Concrete:	ImpactSound = ConcreteSound; break;
	case SHOOTER_SURFACE_Dirt:		ImpactSound = DirtSound; break;
	case SHOOTER_SURFACE_Water:		ImpactSound = WaterSound; break;
	case SHOOTER_SURFACE_Metal:		ImpactSound = MetalSound; break;
	case SHOOTER_SURFACE_Wood:		ImpactSound = WoodSound; break;
	case SHOOTER_SURFACE_Grass:		ImpactSound = GrassSound; break;
	case SHOOTER_SURFACE_Glass:		ImpactSound = GlassSound; break;
	case SHOOTER_SURFACE_Flesh:		ImpactSound = FleshSound; break;
	default:						ImpactSound = DefaultSound; break;
	}*/

	return ImpactSound;
}
