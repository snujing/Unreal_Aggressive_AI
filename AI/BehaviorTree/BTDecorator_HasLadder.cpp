// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_HasLadder.h"
#include "Pirate_AIController.h"
#include "Pirate_Character.h"
#include "MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_HasLadder::UBTDecorator_HasLadder() {
	NodeName = TEXT("HasLadder");
}

bool UBTDecorator_HasLadder::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr) {
		return false;
	}

	auto Pirate = Cast<APirate_Character>(CurrentPawn);
	bResult = Pirate->HasLadder;
	

	return bResult;
}

