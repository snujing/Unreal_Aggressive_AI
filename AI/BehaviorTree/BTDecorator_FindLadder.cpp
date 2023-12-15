// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_FindLadder.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "Pirate_Character.h"
#include "Ladder.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_FindLadder::UBTDecorator_FindLadder() {
	NodeName = TEXT("Find Ladder");
}

bool UBTDecorator_FindLadder::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	bResult = false;

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	UWorld* World = CurrentPawn->GetWorld();
	if (World == nullptr) {
		return false;
	}

	// Find all ladder
	TArray<AActor*> ActorsOfClass;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALadder::StaticClass(), ActorsOfClass);
	
	UE_LOG(LogTemp, Log, TEXT("Ladder Num = %d"), ActorsOfClass.Num());

	for (auto actor : ActorsOfClass) {

		UE_LOG(LogTemp, Log, TEXT("Ladder Name = %s"), *actor->GetName());

		auto Ladder = Cast<ALadder>(actor);
		if (Ladder->IsItem) {
			bResult = true;
			UE_LOG(LogTemp, Log, TEXT("Ladder is Item"));
		}
	}

	AActor* ClosestActor = nullptr;
	float MinDistanceSquared = FLT_MAX;

	for (AActor* Actor : ActorsOfClass){
		if (Actor){
			FVector ActorLocation = Actor->GetActorLocation();
			float DistanceSquared = FVector::DistSquared(ActorLocation, CurrentPawn->GetActorLocation());

			if (DistanceSquared < MinDistanceSquared){

				MinDistanceSquared = DistanceSquared;
				ClosestActor = Actor;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("ClosestLadder")), ClosestActor);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("LadderPos")), ClosestActor->GetActorLocation());

	auto ClosestLadder = Cast<ALadder>(ClosestActor);
	ClosestLadder->HaveToPickUp = true;

	return bResult;
}
