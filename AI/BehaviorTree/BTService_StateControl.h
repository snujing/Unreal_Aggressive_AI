#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StateControl.generated.h"

/**
 * Pirate_AI�� <���� ����> �� <BB ���� �ʱ�ȭ> 
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
	// State�� True , �������� False�� ���� ����
	void ChangeState(FName State);

	// ��� State�� False�� ���� ����
	void AllStateFalse();


private:
	UPROPERTY()
	bool IsInit = false;

	UPROPERTY()
	UBehaviorTreeComponent* MyOwnerComp;

};
