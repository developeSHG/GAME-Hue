# GAME-Hue
[DirectX 3D - Team Project] 휴


동영상(Client)    : https://www.youtube.com/watch?v=-ICA6lMnkI8

동영상(Tool)      : https://www.youtube.com/watch?v=xXEDjC7VByM

장르              : 어드벤처

날짜              : 2019.09.01 ~ 2019.10.01

인원              : 3명(담당 파트 : Tool(Map, Object), 카메라, 이펙트, 충돌, 튜토리얼)

개발 환경         : Visual Studio 2015 (x64)

개발 언어 및 도구  : C++, MFC, DirectX9


======================================================================

* Map Tool, Object Tool
  - 3D 툴을 구현해 큐브 블록 배치 및 지형, 지물(오브젝트) 배치

* Bezier Spline 등을 활용해 6가지 카메라 연출 구현
  - 카메라 이벤트에 따른 행동들을 컴포넌트로 구현해 컴포넌트를 교체하며 카메라 이벤트 진행
  - Observer Pattern을 사용해 뷰 행렬, 투영 행렬 등 데이터 연동

* 빌보드와 Point Sprite를 이용해 렌더링 및 최적화한 이펙트
  - 매니저 클래스를 통해 이펙트를 생성 가능하며 생성 시 매니저 클래스 내부의 파티클 풀에서 여분의 파티클을 가져와 행동 컴포넌트를 삽입하는 식으로 구현

* 충돌
  - Sphere, AABB 충돌을 사용해 지형 오브젝트와 충돌 처리 구현
  - 충돌 처리를 공간에 따라 레이어를 나눠서 플레이어가 모든 오브젝트와 연산하는 것을 피하도록 최적화했음
