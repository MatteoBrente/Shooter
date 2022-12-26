// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

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
}

// Called every frame
void AEnemy::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	if (IsDead())
		this->Destroy();
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

	return DamageApplied;
}

bool AEnemy::IsDead() const
{
	return (Health <= 0.f);
}

void AEnemy::Shoot()
{
	if (!CanShoot)
		return;

	
	SpawnProjectiles();
		
	// Deactivate shooting until the cooldown gets up
	CanShoot = false;
	FTimerHandle ShootHandle;
	GetWorldTimerManager().SetTimer(ShootHandle, this, &AEnemy::ResetShot, ShotCooldown, false);
}

void AEnemy::ResetShot()
{
	CanShoot = true;
}