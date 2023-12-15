// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos() {

	NodeName = TEXT("FindPatrolPos");
}

// BehaviorTree에 사용하는 노드는 반환이 필요(성공, 실패 등등 )
// BehaviorTree에서 노드의 상태에 따라 실행 여부나 방향이 달라질 수 있기 때문
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentController = OwnerComp.GetAIOwner();
	if (CurrentController == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto CurrentPawn = CurrentController->GetPawn();
	if (CurrentPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}

	FNavLocation RandomLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, RandomLocation)) {
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PatrolPos")), RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}