// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "EnemyController.h"

void AShooterGameMode::PawnKilled(APawn* Pawn)
{
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	
	// If the killed pawn is the player, then end the game
	if (PC)
		EndGame(false);

	// Else, check if there are other enemies in the level
	for (AEnemyController* EC : TActorRange<AEnemyController>(GetWorld()))
	{
		if (!EC->IsDead())
			return;
	}

	EndGame(true);
}

void AShooterGameMode::EndGame(bool PlayerIsWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld())) 
	{
		// True if I'm the player and the winner, also true if I'm NOT the player and NOT the winner
		// False in all other cases
		bool IsThisWinner = (Controller->IsPlayerController() == PlayerIsWinner);
		Controller->GameHasEnded(Controller->GetPawn(), IsThisWinner);
	}
}