# [Unreal Portfolio] Unreal_Aggressive_AI

NavLink Proxy와 Smart Links를 사용한 적대적 AI 구현

AI가 상황에 따라 오브젝트를 파괴할 수 있고 지형을 올라갈 수 있는 행동 방식을 만들기 위해 연구


 - 엔진 : Unreal Engine 5.2.1
 - 제작기간 : 2023.04.20 ~ 2023.11.23 
 - 개발규모 : 1인개발


## 데모 동영상

[![Aggressice AI Video](https://i9.ytimg.com/vi/YzTrpclK-2Y/mqdefault.jpg?sqp=CKiXv6wG-oaymwEmCMACELQB8quKqQMa8AEB-AH-CYAC0AWKAgwIABABGHIgUyg5MA8=&rs=AOn4CLA22ZHTmv0z6hFfhD-y-AGzOT6Tuw)](https://youtu.be/YzTrpclK-2Y)


## 플레이어는 언제나 버그를 찾는다

어떤 게임에서는 버그 플레이가 당연하게 사용되기도 한다

게임의 약점을 사용해 플레이를 할 때면 개발자가 이를 보고 "깜짝 놀랄만한 대응책" 만들어 색다른 경험을 주지는 않을까 상상한다

![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/95e64cad-c77c-4393-a1b6-d66c708fdc8c)


## 사다리를 설치해 올라가는 AI

예상치 못한 경험을 주기 위한 방식을 생각하다 "AI가 스스로 사다리를 설치해 올라가면 재미있지 않을까?"

라는 생각으로 프로젝트를 시작하게 되었습니다

![29_](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/2e3e2d24-4042-4a7b-97bf-02dd4cead9f6)

메커니즘은 먼저 UNavigationSystemV1을 사용해 도달할 수 있는 경로가 있는지 체크한다

![29_2](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/15fef845-15f2-413e-b517-2b7d78654b91)

가장 가까이에 있는 사다리를 줍고 EQS를 사용해 사다리 설치 장소를 탐색한다

높이 차이로 생기는 영역과 플레이어 위치를 가중치로 넣어 선정한다

![29_3](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/0a51e7e5-fe16-4f96-bc17-d20942f72c59)

사다리를 설치한 후 바로 올라가 플레이어를 쫓아간다 


## [AI] 전투 상태

플레이어가 전투상태라면 AI도 전투 상태로 전환 / 대치 상태에서 주기적으로 공격을 시도

대치 상태는 AI가 플레이어와 마주보며 거리를 유지

공격을 실행하면 플레이어에게 다가가 1회 공격을 시도

플레이어가 먼저 공격에 성공하면 AI는 피격 모션과 공격 실패로 다시 대치 상태에 돌입

![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/4efe5529-395c-47bc-8d3c-cb6a25a2410d)





## [AI] 사다리 검색 >> 줍기 >> 설치

NavigationSystem을 통해 AI가 플레이어를 쫓아갈 수 없다고 판단되면 실행

가장 가까이 있는 사다리를 찾아서 줍기

사다리를 가지고 있다면 EQS를 사용해 사다리 설치 장소를 탐색

벽의 방향과 높이에 맞추어 사다리를 설치하고 올라간다

![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/2dc8c5cb-463c-428e-a49d-8111f6226ace)



## [Player] 일반/전투 상태에서 움직임

기본 상태에서 마우스 회전과 이동 방향에 따라 캐릭터가 회전

전투 상태에서는 마우스 회전만 캐릭터 회전에 영향

```
void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Player AnimInstance Combat BlenSpace에서 사용
	Horizontal = MovementVector.Y;
	Vertical = MovementVector.X;

	if (Controller != nullptr && CanWalk && IsDrawing) {
		// 전투 상태
		// 캐릭터 방향은 변경되지 않고  이동만 변경 / 이동 방향에 맞추어 캐릭터가 자동 회전 Off
		GetCharacterMovement()->bOrientRotationToMovement = false;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(DirectionX, MovementVector.Y);

		const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(DirectionY, MovementVector.X);
	}
	else if (Controller != nullptr && CanWalk && !IsClimbLadder && !IsDrawing) {
		// 비전투상태
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

```



## [Player] 파쿠르

Trace를 사용해 올라갈 수 있는 지형인지 판단

해당 지형의 높이와 normal 값을 사용해 벽에 달라붙을 수 있도록 구현

![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/9465530d-07e9-4de8-9545-e5e1b1f4c20c)



## [Player] 화염병 던지기

우클릭 Pressed을 통해 화염병을 손에 들고 조준하며 예상 경로를 보여준다

Released 시 마지막 예상 경로로 화염병이 날라가고 지형과 접촉 시 폭발

화염 지대는 Dynamic Navmesh을 통해 AI의 이동에 영향을 준다

![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/d3c71d6f-d211-4565-95af-563b92e68fd3)




## 문제해결(1) - 사다리 설치 방법과 장소 선정

Player의 파쿠르 방식에서 아이디어를 받아 Trace를 통해 지형의 높이와 normal 값을 통해 사다리를 설치

설치 장소는 Player의 위치와 지형의 높이 차이로 생기는 간격을 Trace로 선정 

![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/eb9f1500-af13-4280-956c-c9b5cbde87ef)

```
EBTNodeResult::Type UBTTask_PlaceLadder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	APirate_Character* Pirate_Character = Cast<APirate_Character>(OwnerComp.GetAIOwner()->GetPawn());

	if (Pirate_Character == nullptr) { return EBTNodeResult::Failed; }

	// Ladder 가져오기
	ALadder* ClosestLadder = Cast<ALadder>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("ClosestLadder"))));
  
	// 설치 중에 재설치를 방지
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("IsLadderPlaced")), true);
        
	// 애니메이션 재생
	AnimInstance = Cast<UPirate_AnimInstance>(Pirate_Character->GetMesh()->GetAnimInstance());
	AnimInstance->PlayPlaceLadderMontage();

	return EBTNodeResult::Succeeded;
}
```



## 문제해결(2) - AI 각 행동들의 자연스러운 연계

플레이어가 (높이 점프, 파쿠르, 사다리 타기)등의 행동 시

AI는 Player가 도달할 수 없는 지형에 있다고 판단하여 사다리를 설치하기 위해 움직이는 문제를 발견

Player가 문제가 발생할 수 있는 행동 시 

AI에게 Delay와 지속적인 경로 재탐색을 통해 상황에 맞는 행동을 하도록 변경





## 문제해결(3) - 사다리 설치 정확도 부족

AI가 사다리를 설치하는 장소가 플레이어의 위치와 상이한 문제점을 파악

사다리 설치 위치가 Player를 놓친 위치에서 정지되어 있음을 발견

EQS의 각 지점의 간격을 축소해 정확도를 높이고 Player를 지속적으로 탐색해 설치 장소 최신화













