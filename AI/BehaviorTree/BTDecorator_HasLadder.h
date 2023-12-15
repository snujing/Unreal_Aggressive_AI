// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasLadder.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UBTDecorator_HasLadder : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_HasLadder();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	
};
