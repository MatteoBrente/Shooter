// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	
	// Create the lose screen widget and activate it
	UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
	if (LoseScreen)
		LoseScreen->AddToViewport();

	// Set the restart timer
	FTimerHandle RestartTimer;
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}