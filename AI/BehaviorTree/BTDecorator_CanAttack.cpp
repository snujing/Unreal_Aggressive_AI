// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_CanAttack::UBTDecorator_CanAttack() {
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr) {
		return false;
	}

	// Speed ���� , SearchTarget���� �ٲ�
	if (bResult && (Target->GetDistanceTo(CurrentPawn) <= 200.0f)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("Speed")), 300.0f);
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("Speed")), 450.0f);
	}
	
	// AI�� Target ���� �Ÿ� ����
	return bResult && (Target->GetDistanceTo(CurrentPawn) <= 200.0f);
}
