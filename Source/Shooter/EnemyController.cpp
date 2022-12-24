// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "Kismet/GameplayStatics.h"

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	SetFocus(Player);
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	MoveToActor(Player, 300);
}
