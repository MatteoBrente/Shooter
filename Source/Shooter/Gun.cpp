// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "ShooterProjectile.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/WorldPSCPool.h"

// Sets default values
AGun::AGun()
{
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

	GunBarrel = Cast<UTP_BarrelComponent>(this->GetComponentByClass(UTP_BarrelComponent::StaticClass()));
}

bool AGun::Fire(FVector MuzzlePosition, FRotator MuzzleRotation, AController* Controller)
{
	if (!GunBarrel || !CanShoot)
		return false;

	GunBarrel->Fire(MuzzlePosition, MuzzleRotation, Controller);

	if (GunBarrel->FireSound)
		UGameplayStatics::UGameplayStatics::SpawnSoundAttached
			(GunBarrel->FireSound, GunMesh, TEXT("WeaponSound"));

	
	// Spawn muzzle flash and destroy it after MuzzleTime seconds
	if (GunBarrel->MuzzleFlash)
	{
		CurrentMuzzle =	UGameplayStatics::SpawnEmitterAttached
			(GunBarrel->MuzzleFlash, GunMesh, TEXT("WeaponMuzzle"));
		
		if (CurrentMuzzle)
		{
			FTimerHandle EmitterHandle;
			GetWorldTimerManager().SetTimer(EmitterHandle, this, &AGun::StopMuzzleParticle, MuzzleTime, false);
		}
	}

	// Deactivate shooting until the cooldown gets up
	CanShoot = false;
	FTimerHandle ShootHandle;
	GetWorldTimerManager().SetTimer(ShootHandle, this, &AGun::ResetShot, GunBarrel->ShotCooldown, false);

	return true;
}

void AGun::AddGunComponent(UActorComponent* NewComponent)
{
	// Need to get the component class so that I can call the correct function later
	UClass* SuperClass = NewComponent->GetClass()->GetSuperClass();
	
	if (SuperClass == UTP_BarrelComponent::StaticClass())
		ChangeBarrel(Cast<UTP_BarrelComponent>(NewComponent));
}


void AGun::ChangeBarrel(UTP_BarrelComponent* Barrel)
{
	// Duplicates the barrel so that I can paste it into the gun
	UTP_BarrelComponent* CopiedComponent = DuplicateObject<UTP_BarrelComponent>(Barrel, this);

	if (!CopiedComponent)
		return;
		
	// Destroy the old barrel component
	GunBarrel->DestroyComponent();
	
	// Add the duplicated component and set it as the current barrel
	this->AddInstanceComponent(CopiedComponent);
	GunBarrel = CopiedComponent;
}

void AGun::ResetShot()
{
	CanShoot = true;
}

void AGun::StopMuzzleParticle()
{
	CurrentMuzzle->DestroyComponent();
}