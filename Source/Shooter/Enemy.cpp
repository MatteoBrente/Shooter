// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemy::AEnemy() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay() 
{
	Super::BeginPlay();

	Health = MaxHealth;

	Mesh = GetMesh();
}

// Called every frame
void AEnemy::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	// Calculate the damage. If it's more than the current health, set health to 0
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);

	Health -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("Current Health is equal to: %f"), Health);

	if (IsDead())
	{
		// Detach the collision in order to avoid the player gets caught in it
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SpawnLoot();

		// Destroy the actor after a brief delay
		FTimerHandle DieHandle;
		GetWorldTimerManager().SetTimer(DieHandle, this, &AEnemy::Die, 2.f, false);
	}

	// Notify the game manager that I died, to decide if I should advance the game
	AShooterGameMode* GM = GetWorld()->GetAuthGameMode<AShooterGameMode>();
	if (GM)
	{
		GM->PawnKilled(this);
	}

	return DamageApplied;
}

void AEnemy::SpawnMuzzleParticle()
{
	if (!MuzzleFlash || !Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Non trovo la mesh o il muzzleflash"));
		return;
	}

	CurrentMuzzle = UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle_01"));

	if (CurrentMuzzle)
	{
		FTimerHandle EmitterHandle;
		GetWorldTimerManager().SetTimer(EmitterHandle, this, &AEnemy::StopMuzzleParticle, 0.8f, false);
	}
}

void AEnemy::StopMuzzleParticle()
{
	CurrentMuzzle->DestroyComponent();
}

void AEnemy::Die() 
{
	this->Destroy();
}

bool AEnemy::IsDead() const
{
	return (Health <= 0.f);
}

FRotator AEnemy::AddRandomRotation(FRotator InputRotation, float PlayerSpeed, float Multiplier)
{
	// Add a random rotation on the pitch and yaw based on the player speed magnitude
	float RandomComponent = PlayerSpeed * Multiplier;
	float NewYaw = InputRotation.Yaw + FMath::RandRange(-RandomComponent, RandomComponent);
	
	
	return FRotator{ InputRotation.Pitch, NewYaw, InputRotation.Roll};
}

void AEnemy::SpawnLoot()
{
	if (LootClass)
	{
		AActor* NewObj = GetWorld()->SpawnActor(LootClass, &GetActorTransform());
	}
}