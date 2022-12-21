// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Particles/ParticleSystemComponent.h"
#include "TP_BarrelComponent.h"

#include "Gun.generated.h"

UCLASS()
class SHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:

	/** The muzzle flash that is currently out */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	UParticleSystemComponent* CurrentMuzzle;

	UTP_BarrelComponent* GunBarrel;

private:
	/** The Root component of the various gun components */
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	/** The mesh of the gun */
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* GunMesh;

	/** The duration of the muzzle flash */
	float MuzzleTime = 0.1f;

	/** Checks if the player's shot is off cooldown */
	bool CanShoot = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AGun();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool Fire(FVector MuzzlePosition, FRotator MuzzleRotation, AController* Controller);

	void AddGunComponent(UActorComponent* NewComponent);

	void ChangeBarrel(UTP_BarrelComponent* Barrel);

private:
	// Makes the muzzle stop shooting
	void StopMuzzleParticle();

	void ResetShot();
};
