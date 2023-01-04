// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpawnParticle.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UBTTask_SpawnParticle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpawnParticle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
