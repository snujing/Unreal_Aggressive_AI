#include "BTDecorator_IsPathVaild.h"
#include "Pirate_AIController.h"
#include "MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsPathVaild::UBTDecorator_IsPathVaild() {
	NodeName = TEXT("IsPathVaild");
}

bool UBTDecorator_IsPathVaild::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr) {
		return false;
	}

	bool IsPathVaild = OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName(TEXT("IsPathValid")));
	
	bResult  = IsPathVaild;
	
	UE_LOG(LogTemp, Warning, TEXT("IsPathVaild Decorator = %d"), IsPathVaild);

	return bResult;
}
