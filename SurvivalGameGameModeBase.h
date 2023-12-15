// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASurvivalGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASurvivalGameGameModeBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddToViewport_HP();


	/*----------------------- UClass ���� -----------------------------------*/
	UPROPERTY()
	TSubclassOf<UUserWidget> HUD_Class;


public:
	/*----------------------- ���� ������ -----------------------------------*/
	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMyPlayer* MyCharacter;

};
