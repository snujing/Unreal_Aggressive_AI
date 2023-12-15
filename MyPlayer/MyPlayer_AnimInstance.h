// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyPlayer_AnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackHit)
/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UMyPlayer_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// MyPlayer.cpp JumpOrRoll()에서 제어
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsJumpStart = false;

public:
	UMyPlayer_AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_Drawing();
	UFUNCTION()
	void AnimNotify_Sheath();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_AttackHit();
	UFUNCTION()
	void AnimNotify_RollEnd();
	UFUNCTION()
	void AnimNotify_JumpStart();
	UFUNCTION()
	void AnimNotify_JumpEnd();


	void PlayDrawMontage();
	void PlaySheathMontage();
	void PlayAttackMontage();
	void PlayRollMontage();
	void PlayParryMontage();
	void PlayBlockMontage();
	void PlayBlockingIdleMontage();


private:
	/*------------------------   Animation Blueprint에서 사용  --------------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	int SpeedIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsDrawing1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsDrawing2 = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsBlocking = false;

	// Player Combat BlenSpace에서 사용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	float Horizontal;
	// Player Combat BlenSpace에서 사용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	float Vertical;





	/*-----------------   Montage   ----------------------------------------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* DrawMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* SheathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* ParryMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* BlockingIdleMontage;

	
	UPROPERTY()
	class AMyPlayer* Character;


public:
	FOnAttackHit OnAttackHit;
};
