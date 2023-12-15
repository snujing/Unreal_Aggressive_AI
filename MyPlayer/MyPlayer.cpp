#include "MyPlayer.h"
#include "MyPlayer_AnimInstance.h"
#include "Pirate_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "kismet/GameplayStatics.h"
#include "MyPlayer_Stat_Component.h"
#include "MyPlayer_HUD_Widget.h"
#include "SurvivalGameGameModeBase.h"


AMyPlayer::AMyPlayer()
{
	// 콜리전 크기
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 카메라로 회전하기 때문에, 컨트롤러 회전 차단
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// RotationRate만큼 캐릭터 회전 가속 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// 점프 순간 수직 가속도
	GetCharacterMovement()->JumpZVelocity = 600.f;
	// 낙하 시 이동 제어
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// 아날로그 스틱을 사용한 최소 가속도
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 플레이어 스켈레탈 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/MedievalGirl/Mesh/SK_MedievalGirl.SK_MedievalGirl'"));
	if (SM.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	// 스텟 컴포넌트 생성
	StatComponent = CreateDefaultSubobject<UMyPlayer_Stat_Component>(TEXT("STAT"));

	// HUD UI 생성
	//HUD_Widget = CreateDefaultSubobject<UMyPlayer_HUD_Widget>(TEXT("HUD"));

	// 등(척추) 소켓 찾기
	SpineSocket = FName(TEXT("spine_socket"));
	if (!(GetMesh()->DoesSocketExist(SpineSocket))) {
		UE_LOG(LogTemp, Warning, TEXT("Player has no Spine Socket"));
	}

	// 오른손 소켓 찾기
	HandSocket_R = FName(TEXT("hand_r_socket"));
	if (!(GetMesh()->DoesSocketExist(HandSocket_R))) {
		UE_LOG(LogTemp, Warning, TEXT("Player has no Hand Right Socket"));
	}

	if (!GreatSword) {
		GreatSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GreatSword"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh_GreatSword(TEXT("/Script/Engine.StaticMesh'/Game/Scource/Equipment/Sword/GreatSword.GreatSword'"));
		if (Mesh_GreatSword.Succeeded()) {
			GreatSword->SetStaticMesh(Mesh_GreatSword.Object);
		}
	}

	// 웅크리기 허용
	//GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;



	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

}

void AMyPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// 게임모드 찾기
	ASurvivalGameGameModeBase* MyGameMode = Cast<ASurvivalGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MyGameMode) {
		// Widget_HP 바인딩
		/* Widget 생성을 Level BeginPlay에서 하기 때문에
			바인딩을 BeginPlay에서 해야함 */
		UMyPlayer_HUD_Widget* HUD_Widget = Cast<UMyPlayer_HUD_Widget>(MyGameMode->CurrentWidget);
		HUD_Widget->BindHp(StatComponent);

	}

	// Great Sword 생성 및 장착
	EquipGreatSword(SpineSocket);


}

void AMyPlayer::PostInitializeComponents() {
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMyPlayer_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance) {
		AnimInstance->OnAttackHit.AddUObject(this, &AMyPlayer::AttackCheck);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
void AMyPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyPlayer::JumpOrRoll);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayer::Look);

		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMyPlayer::ToggleCrouch);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMyPlayer::Run);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyPlayer::RunCompleted);

		//Drawing GS
		EnhancedInputComponent->BindAction(DrawGreatSwordAction, ETriggerEvent::Completed, this, &AMyPlayer::DrawGreatSword);
	
		EnhancedInputComponent->BindAction(ZoomInOutAction, ETriggerEvent::Triggered, this, &AMyPlayer::ZoomInOut);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMyPlayer::Attack);


		// 3개는 미구현 
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Completed, this, &AMyPlayer::Parry);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &AMyPlayer::Block);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AMyPlayer::StopBlocking);
	}

}

// 점프
void AMyPlayer::JumpOrRoll(const FInputActionValue& Value) {
	if (CanJumping) {
		//UE_LOG(LogTemp, Warning, TEXT("Jump - 1"));
		//Super::Jump();
		//bIsJumpStart = true;
		AnimInstance->IsJumpStart = true;
	}
	else if (CanRoll) {

		AllStopStateSettings();
		AnimInstance->PlayRollMontage();
		IsRoll = true;
	}
	
}

// 이동
void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Player AnimInstance Combat BlenSpace에서 사용
	Horizontal = MovementVector.Y;
	Vertical = MovementVector.X;

	//UE_LOG(LogTemp, Warning, TEXT("Movement Y = %f, X = %f"), Horizontal, Vertical);

	if (Controller != nullptr && CanWalk && IsDrawing) {

		// 전투 상태
		// 캐릭터 방향은 변경되지 않고  이동만 변경

		// 이동 방향에 맞추어 캐릭터가 자동 회전 Off
		GetCharacterMovement()->bOrientRotationToMovement = false;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, MovementVector.Y);

		const FRotator Rotation2 = Controller->GetControlRotation();
		const FRotator YawRotation2(0, Rotation.Yaw, 0);
		const FVector Direction2 = FRotationMatrix(YawRotation2).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction2, MovementVector.X);

	}
	else if (Controller != nullptr && CanWalk && !IsClimbLadder && !IsDrawing) {
		// 비전투상태
		// 
		// 이동 방향에 맞추어 캐릭터가 자동 회전
		GetCharacterMovement()->bOrientRotationToMovement = true;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	else if (Controller != nullptr && CanWalk && IsClimbLadder) {
		// 사다리를 타고 있다면 위,아래로만 이동가능, 회전 불가능
		const FVector UpVector = GetActorUpVector();
		AddMovementInput(UpVector, MovementVector.Y);


		FVector OppositeForwardVector = -LadderForwardVector;
		FRotator NewPlayerRotation = OppositeForwardVector.Rotation();
		SetActorRotation(NewPlayerRotation);
	}

}

// 카메라 방향 == 컨트롤러 방향
void AMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();


	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);


		// 전투 상태에서 방향에 따른 자동 회전이 꺼져있으므로 직접 돌려줌
		if (IsDrawing) {
			FRotator NewRotation = FRotator(0.0f, GetController()->GetControlRotation().Yaw, 0.0f);
			SetActorRotation(NewRotation);
		}
		
	}

	
}

/* 일단 웅크리기 없음
void AMyPlayer::ToggleCrouch(const FInputActionValue& Value)
{
	IsCrouch = !IsCrouch;

	if (Controller != nullptr)
	{
		if (IsCrouch) {
			Crouch();
		}
		else {
			UnCrouch();
		}

	}
}*/

void AMyPlayer::Run(const FInputActionValue& Value)
{
	if (CanRun){
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
	
}

void AMyPlayer::RunCompleted(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

// Completed E / 대검 장착 or 해제
void AMyPlayer::DrawGreatSword(const FInputActionValue& Value)
{
	if (!CanDraw) {
		return;
	}

	bool value = Value.Get<bool>();
	IsDrawing = !IsDrawing;

	DrawOrSheathStateSettings();

	if (IsDrawing) {
		AnimInstance->PlayDrawMontage();
	}
	else {
		AnimInstance->PlaySheathMontage();
	}
}

void AMyPlayer::ZoomInOut(const FInputActionValue& Value) {
	float value = Value.Get<float>();
	float NewLength = CameraBoom->TargetArmLength - (value * 10.0f);

	CameraBoom->TargetArmLength = FMath::Clamp(NewLength, 50.0f, 1000.0f);
}

// Great Sword 생성 및 장착 // Anim Notify에서도 사용
void AMyPlayer::EquipGreatSword(FName socket) {
	/*
	if (!GreatSword) {
		GreatSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GreatSword"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh_GreatSword(TEXT("/Script/Engine.StaticMesh'/Game/Scource/Equipment/Sword/GreatSword.GreatSword'"));
		if (Mesh_GreatSword.Succeeded()) {
			GreatSword->SetStaticMesh(Mesh_GreatSword.Object);
		}
	}
	*/
	

	GreatSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, socket);
}

// Completed MouseX / 공격
void AMyPlayer::Attack(const FInputActionValue& Value) {
	if (CanAttack) {
		// Action 전부 금지
		AllStopStateSettings();

		// 공격 코드

		// HUD 연결
		//StatComponent->OnAttacked(10.0);

		// 애니메이션
		AnimInstance->PlayAttackMontage();
	}
}

void AMyPlayer::Roll(float Speed)
{
	if (IsRoll) {
		FVector Direction = GetActorForwardVector();
		AddMovementInput(Direction, 1.0);
	}
}

void AMyPlayer::Parry(const FInputActionValue& Value) {
	if (!CanParry) {
		return;
	}

	// Action 전부 금지
	AllStopStateSettings();

	// 애니메이션
	AnimInstance->PlayParryMontage();
}

void AMyPlayer::Block(const FInputActionValue& Value) {
	if (!CanBlock) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Block Start"));


	IsBlocking = true;
	// Action 전부 금지
	//AllStopStateSettings();

	// 애니메이션
	//AnimInstance->PlayBlockMontage();
}

void AMyPlayer::StopBlocking(const FInputActionValue& Value) {
	if (!CanBlock) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Blocking----"));

	IsBlocking = false;
	// Action 전부 금지
	//AllStopStateSettings();

	// 애니메이션
	//AnimInstance->PlayBlockingIdleMontage();
}


// Attack Anim Notify가 Broadcast함
void AMyPlayer::AttackCheck() {
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
		if (HitResult.GetActor()->IsA<APirate_Character>()) {
			auto Pirate = Cast<APirate_Character>(HitResult.GetActor());

			// HitReaction Anim 재생
			Pirate->HitReaction();

			Pirate->OnAttacked(StatComponent->AttackDamage);
			UE_LOG(LogTemp, Log, TEXT("StatComponent->AttackDamage =  %f"), StatComponent->AttackDamage);
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

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 1.f);

}

//전투 상태 셋팅
void AMyPlayer::CombatStateSettings() {
	CanAttack = true;
	CanRun = false;
	CanWalk = true;
	CanRoll = true;
	CanDraw = true;
	CanJumping = false;
	CanGrabbing = false;
	CanClimb = false;
	CanParry = true;
	CanBlock = true;
}

//비전투 상태 셋팅
void AMyPlayer::NonCombatStateSettings() {
	CanAttack = false;
	CanRun = true;
	CanWalk = true;
	CanRoll = false;
	CanDraw = true;
	CanJumping = true;
	CanGrabbing = true;
	CanClimb = true;
	CanParry = false;
	CanBlock = false;
}

//Draw Sheath 상태 셋팅
void AMyPlayer::DrawOrSheathStateSettings() {
	CanAttack = false;
	CanRun = false;
	CanWalk = true;
	CanRoll = false;
	CanDraw = true;
	CanJumping = false;
	CanGrabbing = false;
	CanClimb = false;
	CanParry = false;
	CanBlock = false;
}

//모든 액션 금지 상태 셋팅
void AMyPlayer::AllStopStateSettings() {
	CanAttack = false;
	CanRun = false;
	CanWalk = false;
	CanRoll = false;
	CanDraw = false;
	CanJumping = false;
	CanGrabbing = false;
	CanClimb = false;
	CanParry = false;
	CanBlock = false;

}

// 매달린 상태
void AMyPlayer::GrabStateSettings() {
	CanAttack = false;
	CanRun = false;
	CanWalk = false;
	CanRoll = false;
	CanDraw = false;
	CanJumping =true;
	CanGrabbing = true;
	CanClimb = true;
	CanParry = false;
	CanBlock = false;

}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}