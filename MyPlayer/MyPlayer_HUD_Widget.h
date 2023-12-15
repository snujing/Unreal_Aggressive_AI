// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayer_HUD_Widget.generated.h"

/*HUD = Head Up Display*/
UCLASS()
class SURVIVALGAME_API UMyPlayer_HUD_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHp(class UMyPlayer_Stat_Component* StatComp);
	void UpdateHp();

	void BindAIHp(class APirate_Character* Pirate);


public:
	/** ½ºÅÝ ÄÄÆ÷³ÍÆ® */
	TWeakObjectPtr<class UMyPlayer_Stat_Component> StatComponent;

	/** Pirate */
	TWeakObjectPtr<class  APirate_Character> Pirate_Character;

public:
	/* Player HP ¹ÙÀÎµå À§Á¬ */
	UPROPERTY(meta =(BindWidget))
	class UProgressBar* HP_BAR;

	/* Pirate HP ¹ÙÀÎµå À§Á¬ */
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* AI_HP_BAR;
	
};
