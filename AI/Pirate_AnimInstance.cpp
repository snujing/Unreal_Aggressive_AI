#include "Pirate_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Pirate_Character.h"


// 생성자
UPirate_AnimInstance::UPirate_AnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM1(TEXT("/Script/Engine.AnimMontage'/Game/Pirate/Animations/Pirate_Attack_Montage.Pirate_Attack_Montage'"));
	if (AM1.Succeeded()) {
		AttackMontage = AM1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM2(TEXT("/Script/Engine.AnimMontage'/Game/Pirate/Animations/Pirate_SitIdle_Montage.Pirate_SitIdle_Montage'"));
	if (AM2.Succeeded()) {
		SitIdleMontage = AM2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM3(TEXT("/Script/Engine.AnimMontage'/Game/Pirate/Animations/Pirate_PickUp_Montage.Pirate_PickUp_Montage'"));
	if (AM3.Succeeded()) {
		PickUpLadderMontage = AM3.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM4(TEXT("/Script/Engine.AnimMontage'/Game/Pirate/Animations/Pirate_PlaceLadder_Montage.Pirate_PlaceLadder_Montage'"));
	if (AM4.Succeeded()) {
		PlaceLadderMontage = AM4.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM5(TEXT("/Script/Engine.AnimMontage'/Game/Pirate/Animations/Pirate_HitReaction_Montage.Pirate_HitReaction_Montage'"));
	if (AM5.Succeeded()) {
		HitReactionMontage = AM5.Object;
	}

	

}


void UPirate_AnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {

		// 속도를 사용해
		// idle, walk, run 구분
		Speed = Pawn->GetVelocity().Size();
		if (Speed > 400.0f) {
			SpeedIndex = 2;
		}
		else if (Speed > 50.0f) {
			SpeedIndex = 1;
		}
		else {
			SpeedIndex = 0;
		}

		Character = Cast<APirate_Character>(Pawn);
		if (Character) {
			// Pirate 떨어지는지 체크
			IsFalling = Character->GetMovementComponent()->IsFalling();



			// Pirate 전투 상태 체크
			IsCombat = Character->IsCombat;
		}
	}
}

void UPirate_AnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
	//UE_LOG(LogTemp, Warning, TEXT("Play AttackMontage"));
}

void UPirate_AnimInstance::PlaySitIdleMontage()
{
	Montage_Play(SitIdleMontage, 1.f);
	//UE_LOG(LogTemp, Warning, TEXT("Play SitIdleMontage"));
}

void UPirate_AnimInstance::PlayPickUpLadderMontage() {
	UE_LOG(LogTemp, Warning, TEXT("Play Pick Up Ladder Montage"));
	Montage_Play(PickUpLadderMontage, 1.f);
}

void UPirate_AnimInstance::PlayPlaceLadderMontage() {
	UE_LOG(LogTemp, Warning, TEXT("Play Place Ladder Montage"));
	Montage_Play(PlaceLadderMontage, 1.f);
}

void UPirate_AnimInstance::PlayHitReactionMontage() {
	Montage_Play(HitReactionMontage, 1.f);
}






void UPirate_AnimInstance::AnimNotify_PickUp() {
	UE_LOG(LogTemp, Warning, TEXT("Play Pick Up Ladder Notify"));
	Character->AttachLadderToHand();
	IsPickUpLadder = true;
}



void UPirate_AnimInstance::AnimNotify_Place() {
	UE_LOG(LogTemp, Warning, TEXT("Play Place Ladder Notify"));
	
	Character->PlaceLadder();
	IsPickUpLadder = false;
}

void UPirate_AnimInstance::AnimNotify_AttackHit() {
	Character->AttackCheck();
}