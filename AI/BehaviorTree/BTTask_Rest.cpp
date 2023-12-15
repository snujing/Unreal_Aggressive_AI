// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Rest.h"
#include "Pirate_AnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Pirate_Character.h"

UBTTask_Rest::UBTTask_Rest() {
	NodeName = TEXT("Rest");
}


EBTNodeResult::Type UBTTask_Rest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);



	APirate_Character* Pirate_Character = Cast<APirate_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pirate_Character == nullptr) {
		return EBTNodeResult::Failed;
	}

	AnimInstance = Cast<UPirate_AnimInstance>(Pirate_Character->GetMesh()->GetAnimInstance());


	AnimInstance->PlaySitIdleMontage();


	return EBTNodeResult::Succeeded;
}



