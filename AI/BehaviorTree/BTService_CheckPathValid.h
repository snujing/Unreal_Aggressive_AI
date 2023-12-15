// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPathValid.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UBTService_CheckPathValid : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPathValid();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY()
	class ASurvivalGameGameModeBase* Cur_GameMode;

};
