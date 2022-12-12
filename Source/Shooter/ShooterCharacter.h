// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_WeaponComponent.h"
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

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;
protected:
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	/** Makes the player dash in the forward direction. */
	void OnDash();
	/** Resets the dash when enough time has passed. */
	void ResetDash();

	void ResetShot();

	/** Checks if the player's dash is off cooldown */
	bool CanDash = true;
	/** Checks if the player's shot is off cooldown */
	bool CanShoot = true;

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
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	/** The distance in which the player dashes */
	UPROPERTY(EditAnywhere)
		float DashDistance = 6000.f;
	/** The time between each dash. */
	UPROPERTY(EditAnywhere)
		float DashCooldown = 3.f;
	/** Timer between each shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float ShotCooldown = 0.75f;

private:
	/** The class of gun that should be spawned at the start of the game */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> GunClass;
	
	/** The player's gun object */
	AActor* Gun;
};

