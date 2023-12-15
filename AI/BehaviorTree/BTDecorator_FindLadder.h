#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_FindLadder.generated.h"


UCLASS()
class SURVIVALGAME_API UBTDecorator_FindLadder : public UBTDecorator {
	GENERATED_BODY()
	
	UBTDecorator_FindLadder();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UObject> LadderClass;

};
