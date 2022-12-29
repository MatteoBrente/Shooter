// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameMode.h"

void AShooterGameMode::PawnKilled(APawn* Pawn)
{
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	
	if (PC)
	{
		PC->GameHasEnded(nullptr, false);
	}
}