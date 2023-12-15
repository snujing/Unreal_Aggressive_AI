// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckPathValid.h"
#include "Pirate_Character.h"
#include "Pirate_AIController.h"
#include "AIController.h"
#include "MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SurvivalGameGameModeBase.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AI/Navigation/NavQueryFilter.h"
#include "AI/Navigation/NavigationDataInterface.h"


UBTService_CheckPathValid::UBTService_CheckPathValid() {

	NodeName = TEXT("Check Path Valid");

	// tick 시간 간격 1초
	Interval = 1.0f;



}


void UBTService_CheckPathValid::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// GameMode 가져오기
	AGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode();
	Cur_GameMode = Cast<ASurvivalGameGameModeBase>(CurrentGameMode);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem) {

		auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
		if (CurrentPawn == nullptr) { return; }


		FVector PathStart = CurrentPawn->GetActorLocation();
		FVector PathEnd = Cur_GameMode->MyCharacter->GetActorLocation();
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), PathStart, PathEnd, NULL);

		//UE_LOG(LogTemp, Warning, TEXT("<NavPath> PTr = %d,    Valid = %d,    Partial = %d"), NavPath, NavPath->IsValid(), NavPath->IsPartial());


		if (NavPath->IsValid() && !NavPath->IsPartial())
		{
			UE_LOG(LogTemp, Warning, TEXT("IsPathVaild Service Set Bool =  True"));
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPathValid")), true);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Path can't found!"));
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsPathValid")), false);
		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Service <Check Path Valid> has no UNavigationSystemV1"));
	}



	/*
		FPathFindingQuery QueryParams;
		QueryParams.StartLocation = CurrentPawn->GetActorLocation();
		QueryParams.EndLocation = Cur_GameMode->MyCharacter->GetActorLocation();
		
		//QueryParams.NavData = NavSystem->GetMainNavData();
		//const ANavigationData* NavData = NavSystem->GetNavDataForProps();
		const ANavigationData* NavData = NavSystem->GetAbstractNavData();
		QueryParams.NavData = NavData;

		const AAIController* AIOwner = OwnerComp.GetAIOwner();
		QueryParams.Owner = AIOwner;
		//QueryParams.Owner = CurrentPawn->GetController();

		QueryParams.SetAllowPartialPaths(false);

		//FPathFindingQuery Query(MyNavData, MyNavAgent->GetNavAgentLocation(), NavMeshGoalLocation, QueryFilter);

		bool Result = NavSystem->TestPathSync(QueryParams, EPathFindingMode::Regular);*/

}
