// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Pirate_AnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pirate_Character.h"




UBTTask_Attack::UBTTask_Attack() {
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);



	APirate_Character* Pirate_Character = Cast<APirate_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pirate_Character == nullptr) {
		return EBTNodeResult::Failed;
	}

	AnimInstance = Cast<UPirate_AnimInstance>(Pirate_Character->GetMesh()->GetAnimInstance());


	AnimInstance->PlayAttackMontage();

	// IsAttackTime false�� �ʱ�ȭ
	// Player�� ���� �߿� �ֱ������� ������ �����ϴµ� ���
	// ���� Task�� ����
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsAttackTime")), false);

	
	return EBTNodeResult::Succeeded;
}

