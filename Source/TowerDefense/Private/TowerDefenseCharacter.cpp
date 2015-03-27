// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TowerDefense.h"
#include "TowerDefenseCharacter.h"
#include "TowerDefenseProjectile.h"
#include "Animation/AnimInstance.h"

#include "TowerDefenseGameMode.h"
#include "Units/Tower.h"
#include "Units/Extender.h"
#include "Defines.h"
#include "DrawDebugHelpers.h"
#include "FX/ImpactEffect.h"

//////////////////////////////////////////////////////////////////////////
// ATowerDefenseCharacter

ATowerDefenseCharacter::ATowerDefenseCharacter(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	P_health = 1.f; 
	P_shield = 1.f;
	Is_Tower = false; 
	Is_Monster = false; 
	ViewedObject = ""; 

	// Create a CameraComponent	
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUseControllerViewRotation_DEPRECATED = true; // for backwards compatibility with old saved content
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	
	Weapon = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh1P"));

	Weapon_MinDamages = 10;
	Weapon_MaxDamages = 20;

	Weapon_FireRate = 1 / 7.5f; // 10 Bullets per sec;
	Weapon_CurrentFireElapsedTime = Weapon_FireRate;
	Weapon_IsFiring = false;
	Weapon_IsAiming = false;
	Weapon_WasFiring = false;
	/*Weapon->SetOnlyOwnerSee(true);
	Weapon->AttachSocketName = "ReaperSocket";
	Weapon->AttachParent = Mesh1P;*/

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATowerDefenseCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATowerDefenseCharacter::OnFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATowerDefenseCharacter::OnStopFire);
	InputComponent->BindAction("SecondFire", IE_Pressed, this, &ATowerDefenseCharacter::OnSecondFire);
	InputComponent->BindAction("SecondFire", IE_Released, this, &ATowerDefenseCharacter::OnStopSecondFire);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATowerDefenseCharacter::TouchStarted);

	InputComponent->BindAxis("MoveForward", this, &ATowerDefenseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATowerDefenseCharacter::MoveRight);

	InputComponent->BindAction("SpawnTurret", IE_Pressed, this, &ATowerDefenseCharacter::SpawnTurret);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATowerDefenseCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATowerDefenseCharacter::LookUpAtRate);
}

void ATowerDefenseCharacter::OnFire()
{
	Weapon_IsFiring = true;
}

void ATowerDefenseCharacter::OnStopFire()
{
	Weapon_IsFiring = false;
}

void ATowerDefenseCharacter::OnSecondFire()
{
	Weapon_IsAiming = true;
	// try and play a firing animation if specified
	/*if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			if (!Weapon_IsFiring)
				AnimInstance->Montage_Play(IdleAimAnimation, 1.f);
		}
	}*/
}

void ATowerDefenseCharacter::OnStopSecondFire()
{
	Weapon_IsAiming = false;
	// try and play a firing animation if specified
	/*if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			if (!Weapon_IsFiring)
				AnimInstance->Montage_Play(IdleAnimation, 1.f);
		}
	}*/
}

void ATowerDefenseCharacter::SpawnTurret()
{
	UWorld* World = GetWorld();
	if (World != NULL)
	{
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);

		FVector Start = CameraLoc;
		// you need to add a uproperty to the header file for a float PlayerInteractionDistance
		FVector End = CameraLoc + (CameraRot.Vector() * 1000000);

		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position

		FHitResult outHit;
		FCollisionQueryParams params;
		params.bTraceComplex = true;
		params.bTraceAsyncScene = true;
		params.bReturnPhysicalMaterial = true;


		DrawDebugLine(World, Start, End, FColor::Red, false, 10.f);
		if (World->LineTraceSingle(outHit, Start, End, ECC_Visibility, params))
		{
			UE_LOG(LogTemp, Warning, TEXT("spawn tower"));
			auto* mode = World->GetAuthGameMode<ATowerDefenseGameMode>();
			if (mode != nullptr)
			{
				ATower* tower = mode->Units.instanciateTower(ETower::Gatling, World, outHit.ImpactPoint, FRotator::ZeroRotator);
				UE_LOG(LogTemp, Warning, TEXT("%p"), tower);
			}
		}
		// spawn the projectile at the muzzle

	}
}

void ATowerDefenseCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// only fire for first finger down
	if (FingerIndex == 0)
	{
		OnFire();
	}
}

void ATowerDefenseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATowerDefenseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATowerDefenseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATowerDefenseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ATowerDefenseCharacter::IsTargeting() const
{
	return false;
}

float ATowerDefenseCharacter::GetRunningSpeedModifier() const
{
	return false;
}

bool ATowerDefenseCharacter::IsRunning() const
{
	return false;
}

void ATowerDefenseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Controller && Controller->IsLocalPlayerController()) // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	{
		UpdateLookAtInfos();

		Weapon_CurrentFireElapsedTime += DeltaSeconds;
		if (Weapon_IsFiring && Weapon_CurrentFireElapsedTime > Weapon_FireRate)
		{
			FireWeapon();
			Weapon_CurrentFireElapsedTime = 0;
			Weapon_WasFiring = true;
		}
		else if (!Weapon_IsFiring && Weapon_WasFiring)
		{
			StopFireWeapon();
			Weapon_WasFiring = false;
		}
	}
}

USkeletalMeshComponent* ATowerDefenseCharacter::GetWeaponMesh() const
{
	return Weapon;
}

FVector ATowerDefenseCharacter::GetMuzzleLocation() const
{
	USkeletalMeshComponent* UseMesh = GetWeaponMesh();
	return UseMesh->GetSocketLocation(MuzzleAttachPoint);
}


void ATowerDefenseCharacter::SpawnTrailEffect(const FVector& EndPoint)
{
}

void ATowerDefenseCharacter::SpawnMuzzleEffect()
{
	GetMuzzleLocation();
	if (MuzzlePSC == NULL) 
	{
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Weapon, MuzzleAttachPoint);
		MuzzlePSC->bOwnerNoSee = false;
		MuzzlePSC->bOnlyOwnerSee = true;
	}
}

void ATowerDefenseCharacter::SpawnImpactEffect(const FHitResult& Impact)
{
	if (!Impact.bBlockingHit)
		return;

	if (ImpactTemplate) {
		AImpactEffect* EffectActor = GetWorld()->SpawnActorDeferred<AImpactEffect>(ImpactTemplate, Impact.ImpactPoint, Impact.ImpactNormal.Rotation());
		if (EffectActor)
		{
			EffectActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(EffectActor, FTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint));
		}
	}
}

void ATowerDefenseCharacter::UpdateLookAtInfos()
{
	Is_Tower = false;
	Is_Monster = false;

	FVector CamLoc;
	FRotator CamRot;

	Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
	const FVector StartTrace = CamLoc; // trace start is the camera location
	const FVector Direction = CamRot.Vector();
	const FVector EndTrace = StartTrace + Direction * 20000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	if (GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECC_Camera, TraceParams)) { // simple trace function

		//5Q			UE_LOG(LogTemp, Error, TEXT("Raycast"));

		Is_Monster = false;
		Is_Tower = false;
		ATower* tower = Cast<ATower>(Hit.GetActor()); // we cast the hit actor to the Atower 
		if (tower)
		{
			//UE_LOG(LogTemp, Error, TEXT("Tower")); 
			Is_Tower = true;
			ViewedObject = tower->Name;
			ViewedObject_currentlife = tower->CurrentLife;
			ViewedObject_maxlife = tower->MaxLife;
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("Not Tower")); 
			Is_Tower = false;
			AMonster* monster = Cast<AMonster>(Hit.GetActor()); // we cast the hit actor to the Atower 
			if (monster)
			{
				//UE_LOG(LogTemp, Error, TEXT("Monster"));
				Is_Monster = true;
				ViewedObject = monster->Name;
				ViewedObject_currentlife = monster->CurrentLife;
				ViewedObject_maxlife = monster->MaxLife;
			}
			else
			{
				//UE_LOG(LogTemp, Error, TEXT("Not Monster"));
				Is_Monster = false;
			}

		}
	}
}

void ATowerDefenseCharacter::FireWeapon()
{
	FVector CamLoc;
	FRotator CamRot;

	Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation
	const FVector StartTrace = CamLoc; // trace start is the camera location
	const FVector Direction = CamRot.Vector();
	const FVector EndTrace = StartTrace + Direction * 20000;

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		// spawn the projectile at the muzzle
		//World->SpawnActor<ATowerDefenseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

		// Tir du rayon
		static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

		// Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult Hit(ForceInit);
		World->LineTraceSingle(Hit, StartTrace, EndTrace, ECC_Pawn, TraceParams);
		SpawnMuzzleEffect();
		SpawnTrailEffect(Hit.GetActor() ? Hit.ImpactPoint : EndTrace);
		SpawnImpactEffect(Hit);

		//DrawDebugLine(World, StartTrace, EndTrace, FColor::Green, false, 5);

		AMonster* monster = Cast<AMonster>(Hit.GetActor());
		if (monster)
		{
			float damages = getRandomFloat(Weapon_MinDamages, Weapon_MaxDamages);
			monster->CurrentLife -= damages*monster->Defense[static_cast<int>(EElement::Normal)];
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			/*if (Weapon_IsAiming)
				AnimInstance->Montage_Play(FireAimAnimation, 1.f);
			else*/
				AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ATowerDefenseCharacter::StopFireWeapon()
{
	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			/*if (Weapon_IsAiming)
				AnimInstance->Montage_Play(IdleAimAnimation, 1.f);
			else*/
			AnimInstance->Montage_Stop(1.f);
		}
	}

	if (MuzzlePSC)
	{
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = nullptr;
	}
}