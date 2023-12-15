#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StateControl.generated.h"

/**
 * Pirate_AI의 <상태 제어> 및 <BB 변수 초기화> 
 */
UCLASS()
class SURVIVALGAME_API UBTService_StateControl : public UBTService
{
	GENERATED_BODY()

	UBTService_StateControl();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	void InitState(UBehaviorTreeComponent& OwnerComp);

public:
	// State만 True , 나머지는 False로 상태 변경
	void ChangeState(FName State);

	// 모든 State를 False로 상태 변경
	void AllStateFalse();


private:
	UPROPERTY()
	bool IsInit = false;

	UPROPERTY()
	UBehaviorTreeComponent* MyOwnerComp;

};
