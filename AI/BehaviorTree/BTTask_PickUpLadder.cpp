// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PickUpLadder.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pirate_Character.h"
#include "AIController.h"
#include "Ladder.h"



UBTTask_PickUpLadder::UBTTask_PickUpLadder() {
	NodeName = TEXT("PickUpLadder");
}

EBTNodeResult::Type UBTTask_PickUpLadder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);



	APirate_Character* Pirate_Character = Cast<APirate_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pirate_Character == nullptr) {
		return EBTNodeResult::Failed;
	}

	ALadder* ClosestLadder = Cast<ALadder>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("ClosestLadder"))));

	Pirate_Character->PickUpLadder(ClosestLadder);



	return EBTNodeResult::Succeeded;
}


