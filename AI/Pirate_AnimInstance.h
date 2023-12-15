// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Pirate_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UPirate_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPirate_AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void PlaySitIdleMontage();
	void PlayPickUpLadderMontage();
	void PlayPlaceLadderMontage();
	void PlayHitReactionMontage();

	UFUNCTION()
	void AnimNotify_PickUp();
	UFUNCTION()
	void AnimNotify_Place();
	UFUNCTION()
	void AnimNotify_AttackHit();


private:
	/*------------------------   Animation Blueprint에서 사용  --------------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	int SpeedIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsPickUpLadder;

	// Pirate가 전투 상태인가?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	bool IsCombat;


	/*-----------------   Montage   ----------------------------------------------------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* SitIdleMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* PickUpLadderMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* PlaceLadderMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowprivateAccess = true))
	UAnimMontage* HitReactionMontage;


private:
	UPROPERTY()
	class APirate_Character* Character;
	
};
