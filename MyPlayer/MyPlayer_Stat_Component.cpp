// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer_Stat_Component.h"

// Sets default values for this component's properties
UMyPlayer_Stat_Component::UMyPlayer_Stat_Component()
{

	PrimaryComponentTick.bCanEverTick = false;



	
}

void UMyPlayer_Stat_Component::BeginPlay()
{
	Super::BeginPlay();

	MaxHp = 100.0;
	Hp = 100.0;

	AttackDamage = 30.0;
	
	
}



void UMyPlayer_Stat_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UMyPlayer_Stat_Component::SetHp(float NewHp)
{
	Hp = NewHp;
	if (Hp < 0)
		Hp = 0;

	OnHpChanged.Broadcast();
}

void UMyPlayer_Stat_Component::OnAttacked(float DamageAmount)
{
	int32 NewHp = Hp - DamageAmount;

	SetHp(NewHp);
}

