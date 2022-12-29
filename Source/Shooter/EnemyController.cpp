// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/Blackboardcomponent.h"
#include "Enemy.h"

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	APawn* MyPawn = GetPawn();

	if (!AIBehavior || !MyPawn)
		return;

	RunBehaviorTree(AIBehavior);

	// Sets the enemy pawn as the start location
	if (MyPawn) 
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), MyPawn->GetActorLocation());

	// Get Player Location
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyController::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

bool AEnemyController::IsDead() const
{
	AEnemy* ControlledPawn = Cast<AEnemy>(GetPawn());

	// If you can find the enemy, check if it's dead
	if (ControlledPawn)
		return ControlledPawn->IsDead();
		
	// If you can't find it, then it's obviously dead
	return true;
}