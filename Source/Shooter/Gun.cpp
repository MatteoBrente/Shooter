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

void AGun::Fire(FRotator MuzzleRotation)
{
	if (!GunBarrel)
	{
		UE_LOG(LogTemp, Warning, TEXT("Non trovo il gunbarrel"));
		return;
	}

	GunBarrel->Fire(MuzzleRotation, GunMesh->GetSocketLocation(TEXT("WeaponMuzzle")));

	
	// Try and play the sound if specified
	if (GunBarrel->FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunBarrel->FireSound, GetActorLocation());
	}

	
	if (GunBarrel->MuzzleFlash)
	{
		CurrentMuzzle =	UGameplayStatics::SpawnEmitterAttached(GunBarrel->MuzzleFlash, GunMesh, TEXT("WeaponMuzzle"));
		
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