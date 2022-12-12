// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShooterProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/WorldPSCPool.h"

#include "Gun.generated.h"

UCLASS()
class SHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AShooterProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	/** Maximum offset of the shotgun cone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxConeValue = 4.f;

	/** Number of pellets per shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int PelletNumber = 8;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AGun();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Fire(FRotator MuzzleRotation);

private:
	/**  */
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	/**  */
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;

};
