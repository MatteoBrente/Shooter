// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = "Update Player Location If Seen";
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn || !OwnerComp.GetAIOwner())
		return;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
		Blackboard->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	else
		Blackboard->ClearValue(GetSelectedBlackboardKey());
}