// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyPlayer_Stat_Component.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UMyPlayer_Stat_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyPlayer_Stat_Component();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetHpRatio() { return Hp / MaxHp; }



public:
	void OnAttacked(float DamageAmount);

	// HP Broadcast
	void SetHp(float NewHp);

		
public:
	UPROPERTY(EditAnywhere, Category=Stat)
	float Hp;
	UPROPERTY(EditAnywhere, Category = Stat)
	float AttackDamage;


	FOnHpChanged OnHpChanged;

	/*  저장 정보  */
	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxHp;
};
