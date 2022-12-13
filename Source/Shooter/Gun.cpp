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

	CurrentMuzzle = nullptr;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Fire(FRotator MuzzleRotation)
{
	UWorld* const World = GetWorld();
	
	// Try and fire a projectile
	if (!ProjectileClass || !World)
	{
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

		const FVector SpawnLocation =  GunMesh->GetSocketLocation(TEXT("WeaponMuzzle"));
		World->SpawnActor<AShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}

	// Try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	
	if (MuzzleFlash)
	{
		CurrentMuzzle =	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("WeaponMuzzle"));
		
		if (!CurrentMuzzle)
			return;

		FTimerHandle EmitterHandle;
		GetWorldTimerManager().SetTimer(EmitterHandle, this, &AGun::StopMuzzleParticle, MuzzleTime, false);
	}

}

void AGun::StopMuzzleParticle()
{
	CurrentMuzzle->DestroyComponent();
}