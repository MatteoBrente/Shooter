// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpawnParticle.h"

#include "AIController.h"
#include "Enemy.h"

UBTTask_SpawnParticle::UBTTask_SpawnParticle()
{
	NodeName = TEXT("Spawn Particle");
}

EBTNodeResult::Type UBTTask_SpawnParticle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Owner = OwnerComp.GetAIOwner();

	if (!Owner)
		return EBTNodeResult::Failed;

	AEnemy* Character = Cast<AEnemy>(Owner->GetPawn());

	if (!Character)
		return EBTNodeResult::Failed;

	Character->SpawnMuzzleParticle();

	return EBTNodeResult::Succeeded;
}