// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "ShooterGameMode.h"

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
	{
		SpawnLoot();
		Destroy(); // this
	}
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
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// End the game
	AShooterGameMode* GM = GetWorld()->GetAuthGameMode<AShooterGameMode>();
	if (GM)
	{
		GM->PawnKilled(this);
	}

	return DamageApplied;
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