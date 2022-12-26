// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTER_API AEnemy : public ACharacter 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Applies damage to the actor
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	UFUNCTION(BlueprintPure)
		bool IsDead() const;

	void Shoot();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Shooting")
		void SpawnProjectiles();

private:
	void ResetShot();

public:
	/** The initial health of the actor */
	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;

	/** The current health of the actor */
	UPROPERTY(VisibleAnywhere)
		float Health;

private:
	bool CanShoot = true;

	UPROPERTY(EditAnywhere)
		float ShotCooldown = 5.f;
};
