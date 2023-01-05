// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemySpawner.h"

#include "ShooterGameMode.generated.h"


UCLASS()
class SHOOTER_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PawnKilled(APawn* Pawn);

	// Spawns the nth enemy wave of the said level
	void SpawnEnemyWave();

private:
	void EndGame(bool PlayerIsWinner);

public:
	UPROPERTY(EditDefaultsOnly)
		int MaxWaveNumber;

private:
	int CurrentWaveNumber;

	AEnemySpawner* Spawner;
};
