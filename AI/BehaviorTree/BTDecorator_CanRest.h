// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanRest.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UBTDecorator_CanRest : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CanRest();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	
};
