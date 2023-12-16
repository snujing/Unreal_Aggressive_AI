# [Unreal Portfolio] Unreal_Aggressive_AI

NavLink Proxy와 Smart Links를 사용한 적대적 AI 구현

AI가 상황에 따라 오브젝트를 파괴할 수 있고 지형을 올라갈 수 있는 행동 방식을 만들기 위해 연구


 - 엔진 : Unreal Engine 5.2.1
 - 제작기간 : 2023.04.20 ~ 2023.11.23 
 - 개발규모 : 1인개발


## 데모 동영상


## [Player] 주요 기능


## [AI] 주요 기능


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

벽의 방향과 높이에 맞우어 사다리를 설치하고 올라간다

![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/2dc8c5cb-463c-428e-a49d-8111f6226ace)





## [Player] 파쿠르




![image](https://github.com/snujing/Unreal_Aggressive_AI/assets/57716676/9465530d-07e9-4de8-9545-e5e1b1f4c20c)



## 문제해결(1) - AI 각 행동들의 자연스러운 연계


## 문제해결(2) - 사다리 설치 정확도 부족




