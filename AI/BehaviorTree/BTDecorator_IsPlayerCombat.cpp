// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsPlayerCombat.h"
#include "Pirate_AIController.h"
#include "Pirate_Character.h"
#include "MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsPlayerCombat::UBTDecorator_IsPlayerCombat() {
	NodeName = TEXT("IsPlayerCombat");
}

bool UBTDecorator_IsPlayerCombat::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	auto Player = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Player == nullptr) {
		return false;
	}

	auto Pirate = Cast<APirate_Character>(CurrentPawn);
	
	bResult = Player->IsDrawing;

	UE_LOG(LogTemp, Warning, TEXT("UBTDecorator_IsPlayerCombat"));

	return bResult;
}


