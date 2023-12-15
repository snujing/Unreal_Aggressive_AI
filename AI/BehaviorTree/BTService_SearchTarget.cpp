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

	// tick �ð� ���� 1��
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

	// Pirate�� Speed ���� (�ٸ� ������ �Űܾ���) 
	float Pirate_Speed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("Speed")));
	auto Pirate = Cast<APirate_Character>(CurrentPawn);
	Pirate->WalkingSpeed = Pirate_Speed;
	//UE_LOG(LogTemp, Warning, TEXT("Pirate Walking Speed = %f"), Pirate_Speed); 


	TArray<FOverlapResult> OverlapResults;

	// �̸� ����, ������ �浹�� X, CurrentPawn(�ڱ� �ڽ�)�� ����
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	// bResult=�浿 ����, OverlapResults=�浹 �������
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

			// Player�� ���� �����ϰ� �ִ� MyPlayer���� üũ
			if (MyPlayer && MyPlayer->GetController()->IsPlayerController()) {
				// ------------MyPlayer�� �ֺ��� ����------------------
				
				//BlackBorad Target ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), MyPlayer);

				//DrawDebugSphere(World, Center, SearchRadius, 64, FColor::Green, false, 0.5f);

				// �÷��̾ ���� ����
				if (MyPlayer->IsDrawing) {

					bool IsHPLow = OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName(TEXT("IsHPLow")));
					
					// Pirate ü���� �������� ���� ���·� ��ȯ , ü���� ������ ���������ϴϱ�
					if (!IsHPLow) {
						// Pirate�� ���� ����
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsCombat")), true);
						Pirate->CombatSetting(true);
					}

				}
				// �÷��̾ ������ ����
				else {
					// Pirate�� ������ ����
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsCombat")), false);
					Pirate->CombatSetting(false);
				}



			}
		}
	}
	else {
		// Target null�� ����
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

		//DrawDebugSphere(World, Center, SearchRadius, 64, FColor::Red, false, 0.5f);
	}


}










