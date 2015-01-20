// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TowerDefense.h"
#include "TowerDefenseCharacter.h"
#include "TowerDefenseProjectile.h"
#include "Animation/AnimInstance.h"

#include "TowerDefenseGameMode.h"
#include "BaseUnit.h"
#include "Defines.h"
#include "DrawDebugHelpers.h"


//////////////////////////////////////////////////////////////////////////
// ATowerDefenseCharacter

ATowerDefenseCharacter::ATowerDefenseCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;
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
	InputComponent->BindAction("SecondFire", IE_Pressed, this, &ATowerDefenseCharacter::OnSecondFire);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATowerDefenseCharacter::TouchStarted);

	InputComponent->BindAxis("MoveForward", this, &ATowerDefenseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATowerDefenseCharacter::MoveRight);
	
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
	/*// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<ATowerDefenseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}*/
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
				//ABaseUnit* tow = mode->Towers.SpawnTower(ETower::Gatling, World, outHit.ImpactPoint, FRotator::ZeroRotator);
			}
		}
		// spawn the projectile at the muzzle
		
	}
}

void ATowerDefenseCharacter::OnSecondFire()
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
			UE_LOG(LogTemp, Warning, TEXT("spawn monster"));
			auto* mode = World->GetAuthGameMode<ATowerDefenseGameMode>();
			if (mode != nullptr)
			{
				mode->Units.instanciateMonster(EMonster::Voidling, World, outHit.ImpactPoint, FRotator::ZeroRotator);
				//ABaseUnit* tow = mode->Monsters.SpawnMonster(EMonster::Voidling, World, outHit.ImpactPoint, FRotator::ZeroRotator);
			}
		}
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
