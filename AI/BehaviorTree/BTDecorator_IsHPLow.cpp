// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsHPLow.h"
#include "Pirate_AIController.h"
#include "Pirate_Character.h"
#include "MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsHPLow::UBTDecorator_IsHPLow() {
	NodeName = TEXT("IsHPLow");
}

bool UBTDecorator_IsHPLow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	bResult = false;

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr) {
		return false;
	}

	auto Pirate = Cast<APirate_Character>(CurrentPawn);


	// Pirate 체력 20% 이하라면 Low
	if (Pirate->CurHp / Pirate->MaxHp <= 0.2f) {
		bResult = true;

		Pirate->IsCombat = false;

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsHPLow")), true);

		// Speed 설정 , SearchTarget에서 바꿈
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("Speed")), 450.0f);
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsHPLow")), false);
	}



	return bResult;
}



