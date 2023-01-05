// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include <cmath>

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
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::ActivateWidget(TSubclassOf<class UUserWidget> WidgetClass)
{
	UUserWidget* Widget = CreateWidget(this, WidgetClass);
	if (Widget)
		Widget->AddToViewport();
}

int AShooterPlayerController::GetRemainingDelay()
{
	float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(RestartTimer);

	// If I don't do this, the time remaining goes to -1 even if I use ceil :D
	if (TimeRemaining <= 0.f)
		return 0;
	
	return ceil(TimeRemaining);
}