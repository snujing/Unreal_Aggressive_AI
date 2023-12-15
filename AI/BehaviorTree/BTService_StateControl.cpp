// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_StateControl.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "Pirate_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_StateControl::UBTService_StateControl() {
	NodeName = TEXT("State_Control");
	// tick �ð� ���� 1��
	Interval = 1.0f;
}

void UBTService_StateControl::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return;
	}

	// State �ʱ� ����
	if (!IsInit) {
		InitState(OwnerComp);
	}


}

// State �ʱ� ���� , TickNode���� 1�� ȣ��
void UBTService_StateControl::InitState(UBehaviorTreeComponent& OwnerComp) {
	// ��� ���� ����
	MyOwnerComp = &OwnerComp;

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("Speed")), 300.0f);




	IsInit = true;
	return;
}


void UBTService_StateControl::ChangeState(FName State_Name) {
	// ���� �ʱ�ȭ
	AllStateFalse();

	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(State_Name, true);
}


void UBTService_StateControl::AllStateFalse() {
	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("State_Combat")), false);
	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("State_Rest")), false);
	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("State_Patrol")), false);

}