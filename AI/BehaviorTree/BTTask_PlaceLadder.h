// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlaceLadder.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UBTTask_PlaceLadder : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PlaceLadder();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	/** 애니메이션 */
	UPROPERTY(VisibleAnywhere)
	class UPirate_AnimInstance* AnimInstance;

};
