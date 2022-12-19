// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "ShooterProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/WorldPSCPool.h"

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

	GunBarrel = Cast<UTP_BarrelComponent>(this->GetComponentByClass(UTP_BarrelComponent::StaticClass()));
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::Fire(FVector MuzzlePosition, FRotator MuzzleRotation)
{
	if (!GunBarrel || !CanShoot)
	{
		return false;
	}

	GunBarrel->Fire(MuzzlePosition, MuzzleRotation);

	
	// Try and play the sound if specified
	if (GunBarrel->FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunBarrel->FireSound, GetActorLocation());
	}

	
	if (GunBarrel->MuzzleFlash)
	{
		CurrentMuzzle =	UGameplayStatics::SpawnEmitterAttached(GunBarrel->MuzzleFlash, GunMesh, TEXT("WeaponMuzzle"));
		
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
	UClass* ComponentClass = NewComponent->GetClass();
	
	if (ComponentClass->GetSuperClass() == UTP_BarrelComponent::StaticClass())
	{
		ChangeBarrel(Cast<UTP_BarrelComponent>(NewComponent));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MA CIAO BELLAH"));
	}
}


void AGun::ChangeBarrel(UTP_BarrelComponent* Barrel)
{
	// Copy the referenced component
	UTP_BarrelComponent* CopiedComponent = DuplicateObject<UTP_BarrelComponent>(Barrel, this);

	if (!CopiedComponent)
		return;
		
	// Destroy the old barrel component
	GunBarrel->DestroyComponent();
	
	// Set the copied component as the new barrel
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