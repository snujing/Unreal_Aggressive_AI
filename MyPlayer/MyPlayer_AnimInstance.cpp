// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyPlayer.h"

UMyPlayer_AnimInstance::UMyPlayer_AnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM1(TEXT("/Script/Engine.AnimMontage'/Game/MedievalGirl/Animations/Player_Draw_GS_Montage.Player_Draw_GS_Montage'"));
	if (AM1.Succeeded()) {
		DrawMontage = AM1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM2(TEXT("/Script/Engine.AnimMontage'/Game/MedievalGirl/Animations/Player_Sheath_GS_Montage.Player_Sheath_GS_Montage'"));
	if (AM2.Succeeded()) {
		SheathMontage = AM2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM3(TEXT("/Script/Engine.AnimMontage'/Game/MedievalGirl/Animations/Player_Attack_Montage.Player_Attack_Montage'"));
	if (AM3.Succeeded()) {
		AttackMontage = AM3.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM4(TEXT("/Script/Engine.AnimMontage'/Game/MedievalGirl/Animations/Player_Roll_Montage.Player_Roll_Montage'"));
	if (AM4.Succeeded()) {
		RollMontage = AM4.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM5(TEXT("/Script/Engine.AnimMontage'/Game/MedievalGirl/Animations/Player_Parrying_Montage.Player_Parrying_Montage'"));
	if (AM5.Succeeded()) {
		ParryMontage = AM5.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM6(TEXT("/Script/Engine.AnimMontage'/Game/MedievalGirl/Animations/Player_Blocking_Montage.Player_Blocking_Montage'"));
	if (AM6.Succeeded()) {
		BlockMontage = AM6.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM7(TEXT("/Script/Engine.AnimMontage'/Game/MedievalGirl/Animations/Player_BlockingIdle_Montage.Player_BlockingIdle_Montage'"));
	if (AM7.Succeeded()) {
		BlockingIdleMontage = AM7.Object;
	}


}

void UMyPlayer_AnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {

		// 속도를 사용해
		// idle, walk, run 구분
		Speed = Pawn->GetVelocity().Size();
		if (Speed > 600.0f) {
			SpeedIndex = 2;
		}
		else if (Speed > 300.0f) {
			SpeedIndex = 1;
		}
		else {
			SpeedIndex = 0;
		}

		Character = Cast<AMyPlayer>(Pawn);
		if (Character) {
			// Player 떨어지는지 체크
			IsFalling = Character->GetMovementComponent()->IsFalling();
			
			// Player Crouch 체크
			IsCrouch = Character->bIsCrouched;

			IsDrawing1 = Character->IsDrawing;
			
			//IsJumpStart = Character->bIsJumpStart;

			// Player 방어 체크
			IsBlocking = Character->IsBlocking;

			// Player Combat BlenSpace에서 사용
			Horizontal = Character->Horizontal;
			Vertical = Character->Vertical;
		}

		
	}
}



void UMyPlayer_AnimInstance::AnimNotify_Drawing() {
	Character->EquipGreatSword(Character->HandSocket_R);
	Character->CombatStateSettings();
}

void UMyPlayer_AnimInstance::AnimNotify_Sheath() {
	Character->EquipGreatSword(Character->SpineSocket);
	Character->NonCombatStateSettings();
}

void UMyPlayer_AnimInstance::AnimNotify_AttackEnd() {
	Character->CombatStateSettings();
}

void UMyPlayer_AnimInstance::AnimNotify_AttackHit() {
	//UE_LOG(LogTemp, Warning, TEXT("Attack Hit"));
	OnAttackHit.Broadcast();
}

void UMyPlayer_AnimInstance::AnimNotify_RollEnd() {
	Character->CombatStateSettings();
	Character->IsRoll = false;
}

void UMyPlayer_AnimInstance::AnimNotify_JumpStart() {
	//UE_LOG(LogTemp, Warning, TEXT("Jump start notify"));
	Character->Jump();
}

void UMyPlayer_AnimInstance::AnimNotify_JumpEnd() {
	//UE_LOG(LogTemp, Warning, TEXT("Jump end notify"));
	IsJumpStart = false;
	//Character->bIsJumpStart = false;
}







void UMyPlayer_AnimInstance::PlayDrawMontage()
{
	Montage_Play(DrawMontage, 1.f);
}

void UMyPlayer_AnimInstance::PlaySheathMontage()
{
	Montage_Play(SheathMontage, 1.f);
}

void UMyPlayer_AnimInstance::PlayAttackMontage() {
	Montage_Play(AttackMontage, 1.f);
}

void UMyPlayer_AnimInstance::PlayRollMontage() {
	Montage_Play(RollMontage, 1.f);
}

void UMyPlayer_AnimInstance::PlayParryMontage() {
	Montage_Play(ParryMontage, 1.f);
}

void UMyPlayer_AnimInstance::PlayBlockMontage() {
	Montage_Play(BlockMontage, 1.f);
}

void UMyPlayer_AnimInstance::PlayBlockingIdleMontage() {
	Montage_Play(BlockingIdleMontage, 1.f);
}
