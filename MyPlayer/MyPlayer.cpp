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
	// �ݸ��� ũ��
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// ī�޶�� ȸ���ϱ� ������, ��Ʈ�ѷ� ȸ�� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// RotationRate��ŭ ĳ���� ȸ�� ���� 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// ���� ���� ���� ���ӵ�
	GetCharacterMovement()->JumpZVelocity = 600.f;
	// ���� �� �̵� ����
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// �Ƴ��α� ��ƽ�� ����� �ּ� ���ӵ�
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

	// �÷��̾� ���̷�Ż �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Script/Engine.SkeletalMesh'/Game/MedievalGirl/Mesh/SK_MedievalGirl.SK_MedievalGirl'"));
	if (SM.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	// ���� ������Ʈ ����
	StatComponent = CreateDefaultSubobject<UMyPlayer_Stat_Component>(TEXT("STAT"));

	// HUD UI ����
	//HUD_Widget = CreateDefaultSubobject<UMyPlayer_HUD_Widget>(TEXT("HUD"));

	// ��(ô��) ���� ã��
	SpineSocket = FName(TEXT("spine_socket"));
	if (!(GetMesh()->DoesSocketExist(SpineSocket))) {
		UE_LOG(LogTemp, Warning, TEXT("Player has no Spine Socket"));
	}

	// ������ ���� ã��
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

	// ��ũ���� ���
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

	// ���Ӹ�� ã��
	ASurvivalGameGameModeBase* MyGameMode = Cast<ASurvivalGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MyGameMode) {
		// Widget_HP ���ε�
		/* Widget ������ Level BeginPlay���� �ϱ� ������
			���ε��� BeginPlay���� �ؾ��� */
		UMyPlayer_HUD_Widget* HUD_Widget = Cast<UMyPlayer_HUD_Widget>(MyGameMode->CurrentWidget);
		HUD_Widget->BindHp(StatComponent);

	}

	// Great Sword ���� �� ����
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


		// 3���� �̱��� 
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Completed, this, &AMyPlayer::Parry);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &AMyPlayer::Block);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AMyPlayer::StopBlocking);
	}

}

// ����
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

// �̵�
void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Player AnimInstance Combat BlenSpace���� ���
	Horizontal = MovementVector.Y;
	Vertical = MovementVector.X;

	//UE_LOG(LogTemp, Warning, TEXT("Movement Y = %f, X = %f"), Horizontal, Vertical);

	if (Controller != nullptr && CanWalk && IsDrawing) {

		// ���� ����
		// ĳ���� ������ ������� �ʰ�  �̵��� ����

		// �̵� ���⿡ ���߾� ĳ���Ͱ� �ڵ� ȸ�� Off
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
		// ����������
		// 
		// �̵� ���⿡ ���߾� ĳ���Ͱ� �ڵ� ȸ��
		GetCharacterMovement()->bOrientRotationToMovement = true;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	else if (Controller != nullptr && CanWalk && IsClimbLadder) {
		// ��ٸ��� Ÿ�� �ִٸ� ��,�Ʒ��θ� �̵�����, ȸ�� �Ұ���
		const FVector UpVector = GetActorUpVector();
		AddMovementInput(UpVector, MovementVector.Y);


		FVector OppositeForwardVector = -LadderForwardVector;
		FRotator NewPlayerRotation = OppositeForwardVector.Rotation();
		SetActorRotation(NewPlayerRotation);
	}

}

// ī�޶� ���� == ��Ʈ�ѷ� ����
void AMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();


	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);


		// ���� ���¿��� ���⿡ ���� �ڵ� ȸ���� ���������Ƿ� ���� ������
		if (IsDrawing) {
			FRotator NewRotation = FRotator(0.0f, GetController()->GetControlRotation().Yaw, 0.0f);
			SetActorRotation(NewRotation);
		}
		
	}

	
}

/* �ϴ� ��ũ���� ����
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

// Completed E / ��� ���� or ����
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

// Great Sword ���� �� ���� // Anim Notify������ ���
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

// Completed MouseX / ����
void AMyPlayer::Attack(const FInputActionValue& Value) {
	if (CanAttack) {
		// Action ���� ����
		AllStopStateSettings();

		// ���� �ڵ�

		// HUD ����
		//StatComponent->OnAttacked(10.0);

		// �ִϸ��̼�
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

	// Action ���� ����
	AllStopStateSettings();

	// �ִϸ��̼�
	AnimInstance->PlayParryMontage();
}

void AMyPlayer::Block(const FInputActionValue& Value) {
	if (!CanBlock) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Block Start"));


	IsBlocking = true;
	// Action ���� ����
	//AllStopStateSettings();

	// �ִϸ��̼�
	//AnimInstance->PlayBlockMontage();
}

void AMyPlayer::StopBlocking(const FInputActionValue& Value) {
	if (!CanBlock) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Blocking----"));

	IsBlocking = false;
	// Action ���� ����
	//AllStopStateSettings();

	// �ִϸ��̼�
	//AnimInstance->PlayBlockingIdleMontage();
}


// Attack Anim Notify�� Broadcast��
void AMyPlayer::AttackCheck() {
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	// ó���� blocking�Ȱ� �ϳ��� ��ȯ
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

		// Hit Actor�� Pirate��� Hit Reaction ȣ��
		if (HitResult.GetActor()->IsA<APirate_Character>()) {
			auto Pirate = Cast<APirate_Character>(HitResult.GetActor());

			// HitReaction Anim ���
			Pirate->HitReaction();

			Pirate->OnAttacked(StatComponent->AttackDamage);
			UE_LOG(LogTemp, Log, TEXT("StatComponent->AttackDamage =  %f"), StatComponent->AttackDamage);
		}
		

		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
	}


	/*����׿� ĸ�� �׸���*/
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

//���� ���� ����
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

//������ ���� ����
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

//Draw Sheath ���� ����
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

//��� �׼� ���� ���� ����
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

// �Ŵ޸� ����
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