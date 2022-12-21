// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_BarrelComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UTP_BarrelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	/** Maximum offset of the shotgun cone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxConeValue = 5.f;

	/** Number of pellets per shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int PelletNumber = 5;

	/** The particle system that spawns when the gun is fired */
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
	

	/** Make the weapon Fire Projectile(s) */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
		void Fire(FVector SpawnLocation, FRotator MuzzleRotation, AController* Controller);

	/** Adds a random rotation to the gun's projectiles */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FRotator RandomizeRotation(FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FVector CorrectSpawnPoint(FVector Position);

	/** Timer between each shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float ShotCooldown = 1.f;
};
