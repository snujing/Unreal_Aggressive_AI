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

	// Player Attack이 호출
	void HitReaction();

	// 도끼를 손에 장착
	void TakeInHand(FName socket);

	// Notify가 호출
	void AttachLadderToHand();

	// Notify가 호출
	void PlaceLadder();

	UFUNCTION(BlueprintCallable)
	void PickUpLadder(class ALadder* newLadder);

	UFUNCTION(BlueprintCallable)
	void Attack();

	// 공격 시 콜리전 체크
	UFUNCTION(BlueprintCallable)
	void AttackCheck();

	// 전투, 비전투 상태 변경  Service_SearchTarget이 호출
	void CombatSetting(bool IsCombat);

	// 공격 당함
	void OnAttacked(float DamageAmount);

	// HP Broadcast, 실제 HP 변경은 모두 여기서
	void SetHp(float NewHp);

	// ProgressBar 업데이트
	void UpdateHp();

	float GetHpRatio() { return CurHp / MaxHp; }


	/* 저장 정보 */
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

	// 사다리를 설치할 장소를 찾았는가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasLadderLocation = false;

	// Pirate가 방어를 했는가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBlocking = false;

	// Pirate가 전투 상태인가? AnimInstance에서 사용 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCombat = false;





public:
	/** 애니메이션 */
	UPROPERTY(VisibleAnywhere)
	class UPirate_AnimInstance* AnimInstance;

	/* 바인드 위젯 */
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
