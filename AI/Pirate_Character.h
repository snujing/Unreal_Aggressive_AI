// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pirate_Character.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAIHpChanged);


UCLASS()
class SURVIVALGAME_API APirate_Character : public ACharacter
{
	GENERATED_BODY()

public:
	APirate_Character();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Player Attack�� ȣ��
	void HitReaction();

	// ������ �տ� ����
	void TakeInHand(FName socket);

	// Notify�� ȣ��
	void AttachLadderToHand();

	// Notify�� ȣ��
	void PlaceLadder();

	UFUNCTION(BlueprintCallable)
	void PickUpLadder(class ALadder* newLadder);

	UFUNCTION(BlueprintCallable)
	void Attack();

	// ���� �� �ݸ��� üũ
	UFUNCTION(BlueprintCallable)
	void AttackCheck();

	// ����, ������ ���� ����  Service_SearchTarget�� ȣ��
	void CombatSetting(bool IsCombat);

	// ���� ����
	void OnAttacked(float DamageAmount);

	// HP Broadcast, ���� HP ������ ��� ���⼭
	void SetHp(float NewHp);

	// ProgressBar ������Ʈ
	void UpdateHp();

	float GetHpRatio() { return CurHp / MaxHp; }


	/* ���� ���� */
public:
	UPROPERTY(EditAnywhere, Category = AI)
	float MaxHp;

	UPROPERTY(EditAnywhere, Category = AI)
	float CurHp;

	UPROPERTY(EditAnywhere, Category = AI)
	float AttackDamage;

	UPROPERTY(EditAnywhere, Category = AI)
	float WalkingSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasLadder = false;

	// ��ٸ��� ��ġ�� ��Ҹ� ã�Ҵ°�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasLadderLocation = false;

	// Pirate�� �� �ߴ°�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBlocking = false;

	// Pirate�� ���� �����ΰ�? AnimInstance���� ��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCombat = false;





public:
	/** �ִϸ��̼� */
	UPROPERTY(VisibleAnywhere)
	class UPirate_AnimInstance* AnimInstance;

	/* ���ε� ���� */
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* AI_HP_BAR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Axe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ALadder* Ladder;

public:
	UPROPERTY()
	FName HandSocket_R;

	UPROPERTY()
	FName ScabbardSocket;

	UPROPERTY()
	FName LadderSocket;

	FOnAIHpChanged OnAIHpChanged;

};
