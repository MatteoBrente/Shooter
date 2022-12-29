// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gun.h"
#include "Engine/World.h"

#include "ShooterCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AShooterCharacter();

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Substitutes a gun component for another one */
	UFUNCTION(BlueprintCallable)
		void PickUpGunComponent(UActorComponent* NewComponent);

protected:
	virtual void BeginPlay();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaTime) override;
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	/** Makes the player dash in the forward direction. */
	void OnDash();
	/** Resets the dash when enough time has passed. */
	void ResetDash();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void TurnAtRateMouse(float Rate);

	void LookUpAtRateMouse(float Rate);

	void IncreaseSensitivity();
	void DecreaseSensitivity();

private:
	// Applies damage to the actor
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool IsDead();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	/** The distance in which the player dashes */
	UPROPERTY(EditAnywhere)
		float DashDistance = 6000.f;
	/** The time between each dash. */
	UPROPERTY(EditAnywhere)
		float DashCooldown = 3.f;

	/** The part of the mouse sensitivity that can be changed by the player */
	float MouseSensitivity = 5;

	/** The current health of the player */
	float Health;

	/** The maximum health of the player */
	float MaxHealth = 100;

private:
	/** The class of gun that should be spawned at the start of the game */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass;
	
	/** The player's gun object */
	AGun* Gun;

	/** A multiplier for the mouse sensitivity that lets the player deal with more normal numbers */
	const float MouseMultiplier = 5;

	/** The point where the projectiles spawn. It's here so that it can be centered with the camera */
	UPROPERTY (EditAnywhere)
		FVector ProjectileSpawnPoint = { 30.f, 0.f, 40.f };

	/** Checks if the player's dash is off cooldown */
	bool CanDash = true;
};

