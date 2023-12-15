// Fill out your copyright notice in the Description page of Project Settings.


#include "Pirate_Character.h"
#include "Pirate_AIController.h"
#include "Pirate_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ladder.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/OutputDeviceNull.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyPlayer_HUD_Widget.h"
#include "kismet/GameplayStatics.h"
#include "SurvivalGameGameModeBase.h"
#include "MyPlayer.h"
#include "MyPlayer_Stat_Component.h"

// 생성자
APirate_Character::APirate_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	// AI 컨트롤러 장착
	AIControllerClass = APirate_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// HP, 공격력 설정
	MaxHp = 100.0f;
	CurHp = 100.0f;
	AttackDamage = 10.0f;

	// Speed 조절
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;

	// 오른손 소켓 찾기
	HandSocket_R = FName(TEXT("hand_r_socket"));
	if (!(GetMesh()->DoesSocketExist(HandSocket_R))) {
		UE_LOG(LogTemp, Warning, TEXT("Pirate has no Hand Right Socket"));
	}

	// 검집 소켓 찾기 (도끼 장착 위치)
	ScabbardSocket = FName(TEXT("scabbard_socket"));
	if (!(GetMesh()->DoesSocketExist(ScabbardSocket))) {
		UE_LOG(LogTemp, Warning, TEXT("Pirate has no scabbard socket"));
	}

	// 검집 소켓 찾기 (도끼 장착 위치)
	LadderSocket = FName(TEXT("ladder_socket"));
	if (!(GetMesh()->DoesSocketExist(LadderSocket))) {
		UE_LOG(LogTemp, Warning, TEXT("Pirate has no ladder socket"));
	}

	if (!Axe ) {
		Axe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh_Axe(TEXT("/Script/Engine.StaticMesh'/Game/Scource/Equipment/Axe/Axe.Axe'"));
		if (Mesh_Axe.Succeeded()) {
			Axe->SetStaticMesh(Mesh_Axe.Object);
			//Axe->SetCollisionObjectType(ECC_Visibility);
		}
	}



	//OnAIHpChanged.AddUObject(this, &APirate_Character::UpdateHp);

	
	
}

void APirate_Character::PostInitializeComponents() {
	Super::PostInitializeComponents();

	
}

void APirate_Character::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UPirate_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Pirate No AnimInstance !!"));
	}

	// 게임모드 찾기 
	ASurvivalGameGameModeBase* MyGameMode = Cast<ASurvivalGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MyGameMode) {

		// Widget_HP 바인딩
		/* Widget 생성을 Level BeginPlay에서 하기 때문에
			바인딩을 BeginPlay에서 해야함 */ 
		UMyPlayer_HUD_Widget* HUD_Widget = Cast<UMyPlayer_HUD_Widget>(MyGameMode->CurrentWidget);
		HUD_Widget->BindAIHp(this);
	}

	// Axe 손에 들기
	TakeInHand(FName("Axe"));
}

void APirate_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;

}

void APirate_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// socket_name에 맞는 장비 생성 및 장착
void APirate_Character::TakeInHand(FName equipment) {
	/*
	if (!Axe && equipment == "Axe") {
		Axe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh_Axe(TEXT("/Script/Engine.StaticMesh'/Game/Scource/Equipment/Axe/Axe.Axe'"));
		if (Mesh_Axe.Succeeded()) {
			Axe->SetStaticMesh(Mesh_Axe.Object);
			//Axe->SetCollisionObjectType(ECC_Visibility);
		}
	}
	*/

	Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocket_R);
}


// BT Task에서 실행
void APirate_Character::PickUpLadder(class ALadder* newLadder) {
	UE_LOG(LogTemp, Warning, TEXT("Pirate Pick Up Ladder"));

	// 멤버 변수 최신화
	Ladder = newLadder;
	
	// 상태 변경
	HasLadder = true;
	

	// Montage 실행
	AnimInstance->PlayPickUpLadderMontage();
}


// Anim Notify가 실행
void APirate_Character::AttachLadderToHand() {
	UE_LOG(LogTemp, Warning, TEXT("Pirate Attach Ladder To Hand"));
	
	// 혹시 모르니 1번 더
	Ladder->ConvertItem();
	
	// Attach하기 위해서
	Ladder->LadderStaticMesh->SetSimulatePhysics(false);
	Ladder->LadderStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Ladder->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LadderSocket);
	
}


// Anim Notify가 실행
void APirate_Character::PlaceLadder() {
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("PlaceLadder"), ar, NULL, true);
	HasLadder = false;
}

void APirate_Character::Attack() {
	//GetCharacterMovement()->StopMovementImmediately();
	AnimInstance = Cast<UPirate_AnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->PlayAttackMontage();
}

// Attack Anim Notify가 Broadcast함
void APirate_Character::AttackCheck() {
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	// 처음에 blocking된거 하나만 반환
	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bResult && HitResult.GetActor()) {

		// Hit Actor가 Pirate라면 Hit Reaction 호출
		if (HitResult.GetActor()->IsA<AMyPlayer>()) {
			auto MyPlayer = Cast<AMyPlayer>(HitResult.GetActor());

			// HitReaction Anim 재생
			//Pirate->HitReaction();

			MyPlayer->StatComponent->OnAttacked(AttackDamage);
			UE_LOG(LogTemp, Log, TEXT("StatComponent->AttackDamage =  %f"), AttackDamage);
		}


		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
	}


	/*디버그용 캡슐 그리기*/
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();

	FColor DrawColor;
	if (bResult) {
		DrawColor = FColor::Green;
	}
	else {
		DrawColor = FColor::Red;
	}

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 1.f);

}


// Player Attack이 호출
void APirate_Character::HitReaction() {

	// 방어함
	if (IsBlocking) {

	}
	// 방어 못함
	else if (!IsBlocking) {
		AnimInstance->PlayHitReactionMontage();
	}

}

// 전투, 비전투 상태 변경  Service_SearchTarget이 호출
void APirate_Character::CombatSetting(bool _IsCombat) {
	if (_IsCombat) {
		GetCharacterMovement()->bOrientRotationToMovement = false;
		IsCombat = true;
	}
	else {
		GetCharacterMovement()->bOrientRotationToMovement = true;
		IsCombat = false;
	}
}

// 공격 당함
void APirate_Character::OnAttacked(float DamageAmount) {
	int32 NewHp = CurHp - DamageAmount;

	SetHp(NewHp);
}

// HP Broadcast, 실제 HP 변경은 모두 여기서
void APirate_Character::SetHp(float NewHp) {
	CurHp = NewHp;
	if (CurHp < 0)
		CurHp = 0;

	OnAIHpChanged.Broadcast();
}

void APirate_Character::UpdateHp()
{
	AI_HP_BAR->SetPercent(GetHpRatio());

}
