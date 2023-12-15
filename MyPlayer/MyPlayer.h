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

	/** 애니메이션 */
	UPROPERTY(VisibleAnywhere)
	class UMyPlayer_AnimInstance* AnimInstance;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

public:
	/** 스텟 컴포넌트 */
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





	/*----------------------------  함수  ------------------------------------------------------------*/
public:
	//대검 장착
	void EquipGreatSword(FName socket);

	//전투 상태 셋팅
	void CombatStateSettings();

	//비전투 상태 셋팅
	UFUNCTION(BlueprintCallable)
	void NonCombatStateSettings();

	//Draw Sheath 상태 셋팅
	void DrawOrSheathStateSettings();

	//모든 액션 금지 상태 셋팅
	UFUNCTION(BlueprintCallable)
	void AllStopStateSettings();

	//매달린 상태
	UFUNCTION(BlueprintCallable)
	void GrabStateSettings();


	void Roll(float Speed);

	// 공격 시 콜리전 체크
	void AttackCheck();



	/*--------------------------------------Input Action 함수----------------------------------------------*/
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


	// Player AnimInstance Combat BlenSpace에서 사용
	UPROPERTY()
	float Horizontal = 0.0f;
	// Player AnimInstance Combat BlenSpace에서 사용
	UPROPERTY()
	float Vertical = 0.0f;







	/*--------------------------------------Player State----------------------------------------------*/
	/*------------------------ 해당 Action이 실행 중인가 ---------------------------------------------*/
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




	// 해당 Action이 실행 가능한가
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


	/*--------------새로 넣은 데이터 코드 최신화 필요----------------------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsClimbLadder = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanClimbLadder = false;

	
	/* 저장 정보 */

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

