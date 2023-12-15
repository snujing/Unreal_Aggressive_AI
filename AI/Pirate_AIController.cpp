// Fill out your copyright notice in the Description page of Project Settings.


#include "Pirate_AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


APirate_AIController::APirate_AIController() {

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Boss.BB_Boss'"));
	if (BD.Succeeded()) {
		BlackboardData = BD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Boss.BT_Boss'"));
	if (BT.Succeeded()) {
		BehaviorTree = BT.Object;
	}

}

void APirate_AIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComp = Blackboard.Get();

	if (UseBlackboard(BlackboardData, BlackboardComp)) {
		if (RunBehaviorTree(BehaviorTree)) {
			//todo
		}
	}

}


void APirate_AIController::OnUnPossess() {
	Super::OnUnPossess();


}

