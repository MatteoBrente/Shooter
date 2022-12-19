// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_BarrelComponent.h"

#include "ShooterProjectile.h"

// Sets default values for this component's properties
UTP_BarrelComponent::UTP_BarrelComponent() 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTP_BarrelComponent::BeginPlay() 
{
	Super::BeginPlay();
}


// Called every frame
void UTP_BarrelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FRotator UTP_BarrelComponent::RandomizeRotation(FRotator Rotation)
{
	FRotator NewRotation = Rotation;
		
	NewRotation.Pitch += FMath::RandRange(-MaxConeValue, MaxConeValue);
	NewRotation.Yaw += FMath::RandRange(-MaxConeValue, MaxConeValue);
	
	return NewRotation;
}


FVector UTP_BarrelComponent::CorrectSpawnPoint(FVector Position)
{
	FVector NewPosition = Position;

	NewPosition.Z += 20;

	return NewPosition;
}