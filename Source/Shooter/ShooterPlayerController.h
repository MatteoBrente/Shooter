// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"


UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	void ActivateWidget(TSubclassOf<class UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable)
		int GetRemainingDelay();

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> WinScreenClass;

	/** The blueprint class of the health bar */
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HealthBarClass;

	UPROPERTY(EditAnywhere)
		float RestartDelay = 5.f;

	FTimerHandle RestartTimer;
};