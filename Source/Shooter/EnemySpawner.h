// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"

#include "EnemySpawner.generated.h"

UCLASS()
class SHOOTER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Spawns the enemies
	void SpawnEnemies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemy> EnemyClass;
};
