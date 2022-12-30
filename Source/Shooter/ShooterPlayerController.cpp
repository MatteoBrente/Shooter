// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ActivateWidget(HealthBarClass);
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
		ActivateWidget(WinScreenClass);
	else
		ActivateWidget(LoseScreenClass);

	// Set the restart timer
	FTimerHandle RestartTimer;
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::ActivateWidget(TSubclassOf<class UUserWidget> WidgetClass)
{
	UUserWidget* Widget = CreateWidget(this, WidgetClass);
	if (Widget)
		Widget->AddToViewport();
}