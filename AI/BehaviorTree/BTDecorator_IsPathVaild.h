// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPathVaild.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UBTDecorator_IsPathVaild : public UBTDecorator
{
	GENERATED_BODY()
	

public:
	UBTDecorator_IsPathVaild();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
