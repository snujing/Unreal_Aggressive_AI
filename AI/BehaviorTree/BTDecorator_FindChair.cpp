// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_FindChair.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "Pirate_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BTService.h"

UBTDecorator_FindChair::UBTDecorator_FindChair() {
	NodeName = TEXT("Find Chair");
}

bool UBTDecorator_FindChair::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	UWorld* World = CurrentPawn->GetWorld();
	if (World == nullptr) {
		return false;
	}
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 2000.0f;


	TArray<FOverlapResult> OverlapResults;
	// 이름 없고, 복잡한 충돌은 X, CurrentPawn(자기 자신)은 무시
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	// bResult=충동 개수, OverlapResults=충돌 결과값들
	bool bChairResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bChairResult == false) {
		UE_LOG(LogTemp, Log, TEXT("Cant find Any Object!"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("find Object!"));
	}

	for (const FOverlapResult& Result : OverlapResults)
	{
		FString ActorName = Result.GetActor() ? Result.GetActor()->GetName() : FString(TEXT("None"));
		//UE_LOG(LogTemp, Log, TEXT("Overlap Result - Actor: %s, Component: %s"), *ActorName, *Result.GetComponent()->GetName());
	}

	/*
	if (bResult) {
		for (auto& OverlapResult : OverlapResults) {
			//AMyPlayer* MyPlayer = Cast<AMyPlayer>(OverlapResult.GetActor());

			if (MyPlayer && MyPlayer->GetController()->IsPlayerController()) {
				// Target 설정
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("ChairPos")), MyPlayer);
				bResult = true;
			}
		}
	}
	else {
		// Target null로 설정
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("ChairPos")), FVector(0.f, 0.f, 0.f));
		bResult = false;
	}


	FVector ChairVec = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName(TEXT("ChairPos")));
	UE_LOG(LogTemp, Log, TEXT("Chair Vector : %s"), *FString::Printf(TEXT("(%.2f, %.2f, %.2f)"), ChairVec.X, ChairVec.Y, ChairVec.Z));
	*/
	bResult = true;
	return bResult;
}

