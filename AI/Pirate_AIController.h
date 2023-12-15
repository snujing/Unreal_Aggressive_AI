// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Pirate_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API APirate_AIController : public AAIController
{
	GENERATED_BODY()

public:
	APirate_AIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	void RandomMove();


private:
	FTimerHandle TimerHandle;

	UPROPERTY()
		class UBehaviorTree* BehaviorTree;

	UPROPERTY()
		class UBlackboardData* BlackboardData;


};
