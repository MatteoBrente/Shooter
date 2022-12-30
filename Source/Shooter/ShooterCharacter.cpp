// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterCharacter.h"
#include "ShooterProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "ShooterGameMode.h"


//////////////////////////////////////////////////////////////////////////
// AShooterCharacter

AShooterCharacter::AShooterCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void AShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	Health = MaxHealth;

	// Spawn the gun object
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	
	// Attach the gun to the player
	if (Gun)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		Gun->AttachToComponent(Mesh1P, AttachmentRules, TEXT("GripPoint"));
	}
}

//////////////////////////////////////////////////////////////////////////// Input
void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AShooterCharacter::OnPrimaryAction);

	// Bind dash event
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AShooterCharacter::OnDash);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AShooterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AShooterCharacter::TurnAtRateMouse);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &AShooterCharacter::LookUpAtRateMouse);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AShooterCharacter::LookUpAtRate);

	// Placeholder sensitivity controls
	PlayerInputComponent->BindAction("SensitivityUp", IE_Released, this, &AShooterCharacter::IncreaseSensitivity);
	PlayerInputComponent->BindAction("SensitivityDown", IE_Released, this, &AShooterCharacter::DecreaseSensitivity);
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShooterCharacter::OnPrimaryAction()
{
	if (!Gun)
		return;

	// Calculate the rotation and position for the projectile to spawn in
	AController* ControllerRef = GetController();
	FVector ProjLocation = GetActorLocation() + ProjectileSpawnPoint;
	FRotator ProjRotation = Cast<APlayerController>(ControllerRef)->PlayerCameraManager->GetCameraRotation();
	
	if (Gun->Fire(ProjLocation, ProjRotation, ControllerRef))
	{
		if (!Gun->GunBarrel || !Gun->GunBarrel->FireAnimation)
			return;
		
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
		if (AnimInstance)
			AnimInstance->Montage_Play(Gun->GunBarrel->FireAnimation, 1.f);
	}
}

void AShooterCharacter::OnDash()
{	
	// Check if the dash is off cooldown
	if (!CanDash)
		return;

	// Execute the dash in the direction of the player movement (so you can dash sideways)
	LaunchCharacter(this->GetLastMovementInputVector() * DashDistance, true, true);

	// Start the cooldown of the dash
	CanDash = false;
	FTimerHandle DashHandle;
	GetWorldTimerManager().SetTimer(DashHandle, this, &AShooterCharacter::ResetDash, DashCooldown, false);
}

void AShooterCharacter::ResetDash()
{
	CanDash = true;
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::TurnAtRateMouse(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * MouseSensitivity * MouseMultiplier * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRateMouse(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * MouseSensitivity * MouseMultiplier * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::IncreaseSensitivity()
{
	MouseSensitivity++;
}

void AShooterCharacter::DecreaseSensitivity()
{
	if (MouseSensitivity > 1)
		MouseSensitivity--;
}


//////////////////////////////////////////////////////////// UTILITIES

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// Calculate the damage. If it's more than the current health, set health to 0
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);

	Health -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("Current Player Health is equal to: %f"), Health);

	if (IsDead())
	{
		// End the game
		AShooterGameMode* GM = GetWorld()->GetAuthGameMode<AShooterGameMode>();
		if (GM)
		{
			GM->PawnKilled(this);
		}

		// Deactivate controls and collisions
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageApplied;
}

bool AShooterCharacter::IsDead()
{
	return (Health <= 0);
}

void AShooterCharacter::PickUpGunComponent(UActorComponent* NewComponent)
{
	Gun->AddGunComponent(NewComponent);
}

float AShooterCharacter::GetHealthPercent()
{
	return Health / MaxHealth;
}