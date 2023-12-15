// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer_HUD_Widget.h"
#include "MyPlayer_Stat_Component.h"
#include "Components/ProgressBar.h"
#include "Pirate_Character.h"
#include "SurvivalGameGameModeBase.h"
#include "kismet/GameplayStatics.h"
#include "MyPlayer.h"

void UMyPlayer_HUD_Widget::BindHp(class UMyPlayer_Stat_Component* StatComp)
{
	StatComponent = StatComp;
	UE_LOG(LogTemp, Warning, TEXT("UMyPlayer_HUD_Widget::BindHp = %s"), *StatComponent->GetName());
	// StatComp�� ��������Ʈ�� Widget�� �Լ��� �����ϴ� ��,
	// StatComp���� Widget �Լ��� �����ϴ��� OR 
	// Widget���� StatComp�� ��������Ʈ�� �����ϴ���
	StatComponent->OnHpChanged.AddUObject(this, &UMyPlayer_HUD_Widget::UpdateHp);

}

void UMyPlayer_HUD_Widget::BindAIHp(class APirate_Character* Pirate) {
	Pirate_Character = Pirate;
	Pirate_Character->OnAIHpChanged.AddUObject(this, &UMyPlayer_HUD_Widget::UpdateHp);
}

void UMyPlayer_HUD_Widget::UpdateHp()
{
	UE_LOG(LogTemp, Warning, TEXT("UMyPlayer_HUD_Widget::UpdateHp"));


	if (Pirate_Character.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Pirate_Character.IsValid"));
		AI_HP_BAR->SetPercent(Pirate_Character->GetHpRatio());
		UE_LOG(LogTemp, Warning, TEXT("Pirate_Character->GetHpRatio() = %f"), Pirate_Character->GetHpRatio());
	}
			
	if (StatComponent.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("StatComponent.IsValid()"));
		HP_BAR->SetPercent(StatComponent->GetHpRatio());
	}


}
