# [Unreal Portfolio] Unreal_Aggressive_AI

NavLink Proxy와 Smart Links를 사용한 적대적 AI 구현

AI가 상황에 따라 오브젝트를 파괴할 수 있고 지형을 올라갈 수 있는 행동 방식을 만들기 위해 연구


 - 엔진 : Unreal Engine 5.2.1
 - 제작기간 : 2023.04.20 ~ 2023.11.23 
 - 개발규모 : 1인개발


## 데모 동영상



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




## 문제해결(2) - AI 각 행동들의 자연스러운 연계

플레이어가 (높이 점프, 파쿠르, 사다리 타기)등의 행동 시

AI는 Player가 도달할 수 없는 지형에 있다고 판단하여 사다리를 설치하기 위해 움직이는 문제를 발견

Player가 문제가 발생할 수 있는 행동 시 

AI에게 Delay와 지속적인 경로 재탐색을 통해 상황에 맞는 행동을 하도록 변경



## 문제해결(3) - 사다리 설치 정확도 부족

AI가 사다리를 설치하는 장소가 플레이어의 위치와 상이



