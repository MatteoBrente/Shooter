// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_BarrelComponent.h"

#include "ShooterProjectile.h"

// Sets default values for this component's properties
UTP_BarrelComponent::UTP_BarrelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTP_BarrelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTP_BarrelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UTP_BarrelComponent::Fire(FRotator MuzzleRotation, FVector SpawnLocation)
{
	UWorld* const World = GetWorld();

	// Try and fire a projectile
	if (!ProjectileClass || !World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ciao"));
		return;
	}

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the projectile at the muzzle with a random rotation
	for (int i = 0; i < PelletNumber; i++)
	{
		FRotator SpawnRotation =
		{
			MuzzleRotation.Pitch + FMath::RandRange(-MaxConeValue, MaxConeValue),
			MuzzleRotation.Yaw + FMath::RandRange(-MaxConeValue, MaxConeValue),
			MuzzleRotation.Roll
		};

		SpawnLocation.Z += 10;

		World->SpawnActor<AShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
}
