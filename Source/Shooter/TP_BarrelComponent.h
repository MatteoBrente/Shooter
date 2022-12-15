// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_BarrelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UTP_BarrelComponent : public UActorComponent
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

	/** Maximum offset of the shotgun cone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxConeValue = 4.f;

	/** Number of pellets per shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int PelletNumber = 8;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UTP_BarrelComponent();


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Fire(FRotator MuzzleRotation, FVector SpawnLocation);
		
};
