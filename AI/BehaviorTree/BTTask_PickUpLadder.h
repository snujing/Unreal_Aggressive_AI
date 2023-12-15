// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PickUpLadder.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UBTTask_PickUpLadder : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PickUpLadder();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	
};
