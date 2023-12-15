// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanRest.h"
#include "Pirate_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pirate_Character.h"

UBTDecorator_CanRest::UBTDecorator_CanRest() {
	NodeName = TEXT("CanRest");
}


bool UBTDecorator_CanRest::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	// 환경 변수를 판단해
	// Patrol을 할지, Rest를 할지 판단한다.
	bResult = false;
	
	return bResult;
}
