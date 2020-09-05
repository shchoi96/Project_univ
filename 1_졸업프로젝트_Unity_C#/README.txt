졸업 프로젝트
프로젝트 명 : Raid Hunter
수행 기간 : 2019년 10월 ~ 2020년 05월
개발 도구 및 개발 환경 : Unity3D, Visual Studio
협업 도구 : Unity Collaborate
수행 인원 : 3명
사용 언어 : C#
주요 역할 : 팀장, 프로젝트 관리, 몬스터 설계 및 구현, 게임 구조 설계, 네트워크
주    제   : Mobile MMORPG에서 레이드 몬스터 AI 설계

영상 링크 (마지막 ~ 처음 순)
시연 영상 : https://youtu.be/yxJe2gpkGoA
최종 발표 : https://youtu.be/VddlW--p6do
진도 보고 2차 : https://youtu.be/nlZXYdG6A88
진도 보고 1차 : https://youtu.be/hcAZARNJu9M
제안서 발표 : https://youtu.be/vbefqHS4qvo
주제 발표 : https://youtu.be/_eBpylmZZ-Y

기여부분( 여러 소스 파일 중 기여도가 높은 소스 파일을 첨부했습니다. )
----- RaidMonster
Particle 및 Sound와 관련한 부분을 제외한, 레이드 몬스터의 행동 함수들과 피격, 타격 함수, 공격 범위 표현 함수 등이 존재합니다. 
레이드 몬스터의 행동으로는 페이즈 변화(ChangePhase), Trace, Idle, SearchPlayer, Attack, Skill 등이 있습니다. 구현된 함수는 Unity Asset Store의 비주얼 스크립팅을 가능하게 해주는 Behavior Designer를 이용해서 행동 트리 구현에 사용했습니다.
----- GameManager
게임 내의 플레이어 캐릭터와 레이드 몬스터, 체력바 등을 관리하고 UI 정보를 갱신합니다. 
플레이어 캐릭터와 레이드 몬스터는 GameManager를 통해 서로를 인지합니다.
----- LivingEntity
플레이어 캐릭터와 레이드 몬스터의 기본적인 뼈대
네트워크 동기화를 가지는 체력 갱신, 데미지 적용 함수가 존재합니다.
----- PhotonInit
프로젝트는 Photon Unity Networking을 이용해서 네트워크 동기화를 이루었습니다. PhotonInit은 Photon 네트워크 구성을 설정하는 스크립트이며, Room 입장 시 캐릭터 생성 및 초기화의 기능을 가지고 있습니다.
----- PlayerInput
사용자로부터 모바일 화면을 통한 버튼 입력 또는 키보드 입력을 받아 그에 맞는 값을 설정합니다.
로컬 플레이어가 아닌 경우 영향을 받지 않으며, 플레이어 캐릭터는 이 스크립트에 설정된 값을 이용해 행동을 수행합니다.
----- PlayerCharacter ( 네트워크 관련 및 로직 수정 )
공격, 체력 회복, 회피 등의 함수가 존재합니다. PlayerInput의 값에 따라 각 행동을 수행합니다.
이 스크립트에서는 네트워크 관련 작업과 버그 수정을 위한 로직 수정을 했습니다.
