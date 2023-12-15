// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_HasLadderLocation.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_HasLadderLocation::UBTDecorator_HasLadderLocation() {
	NodeName = TEXT("HasLadderLocation");
}

bool UBTDecorator_HasLadderLocation::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr) {
		return false;
	}

	return bResult;
}

