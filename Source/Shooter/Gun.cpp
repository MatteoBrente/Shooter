// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	MuzzleOffset = GunMesh->GetSocketLocation(TEXT("WeaponMuzzle"));
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Fire(FRotator MuzzleRotation)
{
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the projectile at the muzzle with a random rotation
			for (int i = 0; i < PelletNumber; i++)
			{
				FRotator SpawnRotation =
				{
					MuzzleRotation.Pitch + FMath::RandRange(-MaxConeValue, MaxConeValue),
					MuzzleRotation.Yaw + FMath::RandRange(-MaxConeValue, MaxConeValue),
					MuzzleRotation.Roll
				};

				const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

				World->SpawnActor<AShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}

			if (MuzzleFlash)
			{
				UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("WeaponMuzzle"));
			}
		}
	}
}