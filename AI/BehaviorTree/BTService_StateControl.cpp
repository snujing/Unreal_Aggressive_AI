// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_StateControl.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "Pirate_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_StateControl::UBTService_StateControl() {
	NodeName = TEXT("State_Control");
	// tick 시간 간격 1초
	Interval = 1.0f;
}

void UBTService_StateControl::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return;
	}

	// State 초기 셋팅
	if (!IsInit) {
		InitState(OwnerComp);
	}


}

// State 초기 셋팅 , TickNode에서 1번 호출
void UBTService_StateControl::InitState(UBehaviorTreeComponent& OwnerComp) {
	// 멤버 변수 저장
	MyOwnerComp = &OwnerComp;

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("Speed")), 300.0f);




	IsInit = true;
	return;
}


void UBTService_StateControl::ChangeState(FName State_Name) {
	// 상태 초기화
	AllStateFalse();

	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(State_Name, true);
}


void UBTService_StateControl::AllStateFalse() {
	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("State_Combat")), false);
	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("State_Rest")), false);
	MyOwnerComp->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("State_Patrol")), false);

}