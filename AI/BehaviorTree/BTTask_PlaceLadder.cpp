// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlaceLadder.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pirate_Character.h"
#include "Pirate_AnimInstance.h"
#include "AIController.h"
#include "Ladder.h"



UBTTask_PlaceLadder::UBTTask_PlaceLadder() {
	NodeName = TEXT("PlaceLadder");
}

EBTNodeResult::Type UBTTask_PlaceLadder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	APirate_Character* Pirate_Character = Cast<APirate_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pirate_Character == nullptr) {
		return EBTNodeResult::Failed;
	}

	// Ladder 가져오기
	ALadder* ClosestLadder = Cast<ALadder>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("ClosestLadder"))));

	// IsLadderPlaced = true;
	// 설치 중에 재설치를 방지
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLadderPlaced")), true);


	// 애니메이션 재생
	AnimInstance = Cast<UPirate_AnimInstance>(Pirate_Character->GetMesh()->GetAnimInstance());
	AnimInstance->PlayPlaceLadderMontage();


	// 실제로 벽에 설치는 함수는 Notify가 호출


	return EBTNodeResult::Succeeded;
}



