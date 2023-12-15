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

	// ȯ�� ������ �Ǵ���
	// Patrol�� ����, Rest�� ���� �Ǵ��Ѵ�.
	bResult = false;
	
	return bResult;
}
