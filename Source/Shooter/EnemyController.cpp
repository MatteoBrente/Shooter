// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/Blackboardcomponent.h"

void AEnemyController::BeginPlay() {
	Super::BeginPlay();

	if (AIBehavior) {
		RunBehaviorTree(AIBehavior);

		APawn* MyPawn = GetPawn();

		if (MyPawn) {
			GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), MyPawn->GetActorLocation());
		}

		// Get Player Location
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
}

void AEnemyController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (Player && LineOfSightTo(Player)) {
		SetFocus(Player);

		FVector PlayerLocation = Player->GetActorLocation();
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerLocation);
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastPlayerLocation"), PlayerLocation);
	}
	else {
		ClearFocus(EAIFocusPriority::Gameplay);

		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
}
