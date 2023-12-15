// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "Pirate_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BTService.h"

UBTService_SearchTarget::UBTService_SearchTarget() {
	
	NodeName = TEXT("SearchTarget");

	// tick 시간 간격 1초
	Interval = 1.0f;
}


void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return;
	}

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 800.0f;

	if (World == nullptr) {
		return;
	}

	// Pirate의 Speed 적용 (다른 곳으로 옮겨야함) 
	float Pirate_Speed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("Speed")));
	auto Pirate = Cast<APirate_Character>(CurrentPawn);
	Pirate->WalkingSpeed = Pirate_Speed;
	//UE_LOG(LogTemp, Warning, TEXT("Pirate Walking Speed = %f"), Pirate_Speed); 


	TArray<FOverlapResult> OverlapResults;

	// 이름 없고, 복잡한 충돌은 X, CurrentPawn(자기 자신)은 무시
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	// bResult=충동 개수, OverlapResults=충돌 결과값들
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult) {
		for (auto& OverlapResult : OverlapResults) {
			AMyPlayer* MyPlayer = Cast<AMyPlayer>(OverlapResult.GetActor());

			// Player가 직접 조종하고 있는 MyPlayer인지 체크
			if (MyPlayer && MyPlayer->GetController()->IsPlayerController()) {
				// ------------MyPlayer가 주변에 존재------------------
				
				//BlackBorad Target 설정
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), MyPlayer);

				//DrawDebugSphere(World, Center, SearchRadius, 64, FColor::Green, false, 0.5f);

				// 플레이어가 전투 상태
				if (MyPlayer->IsDrawing) {

					bool IsHPLow = OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName(TEXT("IsHPLow")));
					
					// Pirate 체력이 많을때만 전투 상태로 전환 , 체력이 없으면 도망가야하니까
					if (!IsHPLow) {
						// Pirate도 전투 셋팅
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsCombat")), true);
						Pirate->CombatSetting(true);
					}

				}
				// 플레이어가 비전투 상태
				else {
					// Pirate도 비전투 셋팅
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsCombat")), false);
					Pirate->CombatSetting(false);
				}



			}
		}
	}
	else {
		// Target null로 설정
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

		//DrawDebugSphere(World, Center, SearchRadius, 64, FColor::Red, false, 0.5f);
	}


}










