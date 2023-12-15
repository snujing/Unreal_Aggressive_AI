#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyPlayer.generated.h"


UCLASS(config = Game)
class SURVIVALGAME_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

	/*--------------------------------------Component----------------------------------------------*/

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** �ִϸ��̼� */
	UPROPERTY(VisibleAnywhere)
	class UMyPlayer_AnimInstance* AnimInstance;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

public:
	/** ���� ������Ʈ */
	UPROPERTY(VisibleAnywhere)
	class UMyPlayer_Stat_Component* StatComponent;

	/** UI HUD */
	//UPROPERTY(VisibleAnywhere)
	//class UMyPlayer_HUD_Widget* HUD_Widget;



	/*--------------------------------------Input Action----------------------------------------------*/

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* DrawGreatSwordAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ZoomInOutAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ParryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BlockAction;





	/*----------------------------  �Լ�  ------------------------------------------------------------*/
public:
	//��� ����
	void EquipGreatSword(FName socket);

	//���� ���� ����
	void CombatStateSettings();

	//������ ���� ����
	UFUNCTION(BlueprintCallable)
	void NonCombatStateSettings();

	//Draw Sheath ���� ����
	void DrawOrSheathStateSettings();

	//��� �׼� ���� ���� ����
	UFUNCTION(BlueprintCallable)
	void AllStopStateSettings();

	//�Ŵ޸� ����
	UFUNCTION(BlueprintCallable)
	void GrabStateSettings();


	void Roll(float Speed);

	// ���� �� �ݸ��� üũ
	void AttackCheck();



	/*--------------------------------------Input Action �Լ�----------------------------------------------*/
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ToggleCrouch(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void RunCompleted(const FInputActionValue& Value);
	void DrawGreatSword(const FInputActionValue& Value);
	void ZoomInOut(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void JumpOrRoll(const FInputActionValue& Value);
	void Parry(const FInputActionValue& Value);
	void Block(const FInputActionValue& Value);
	void StopBlocking(const FInputActionValue& Value);





	AMyPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void PostInitializeComponents() override;






public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY()
	float RunningSpeed = 1200.0f;
	UPROPERTY()
	float WalkingSpeed = 500.0f;


	// Player AnimInstance Combat BlenSpace���� ���
	UPROPERTY()
	float Horizontal = 0.0f;
	// Player AnimInstance Combat BlenSpace���� ���
	UPROPERTY()
	float Vertical = 0.0f;







	/*--------------------------------------Player State----------------------------------------------*/
	/*------------------------ �ش� Action�� ���� ���ΰ� ---------------------------------------------*/
	UPROPERTY()
	bool IsCrouch = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDrawing = false;
	UPROPERTY(VisibleAnywhere)
	bool IsAttacking = false;
	UPROPERTY(VisibleAnywhere)
	bool IsRoll = false;
	UPROPERTY(VisibleAnywhere)
	bool bIsJumpStart = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsSlowMotion = false;
	UPROPERTY(VisibleAnywhere)
	bool IsBlocking = false;




	// �ش� Action�� ���� �����Ѱ�
	UPROPERTY(VisibleAnywhere)
	bool CanAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanRun = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanWalk = true;
	UPROPERTY(VisibleAnywhere)
	bool CanRoll = false;
	UPROPERTY(VisibleAnywhere)
	bool CanDraw = true;
	UPROPERTY(VisibleAnywhere)
	bool CanJumping = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanGrabbing = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanClimb = true;
	UPROPERTY(VisibleAnywhere)
	bool CanParry = false;
	UPROPERTY(VisibleAnywhere)
	bool CanBlock = false;


	/*--------------���� ���� ������ �ڵ� �ֽ�ȭ �ʿ�----------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsClimbLadder = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanClimbLadder = false;

	
	/* ���� ���� */

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GreatSword;


public:
	UPROPERTY()
	FName HandSocket_R;
	UPROPERTY()
	FName SpineSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LadderForwardVector;

};

