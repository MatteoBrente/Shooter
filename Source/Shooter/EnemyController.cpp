// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/Blackboardcomponent.h"

void AEnemyController::BeginPlay() 
{
	Super::BeginPlay();

	APawn* MyPawn = GetPawn();

	if (!AIBehavior || !MyPawn)
		return;

	RunBehaviorTree(AIBehavior);

	if (MyPawn) 
	{
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), MyPawn->GetActorLocation());
	}

	// Get Player Location
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyController::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}
