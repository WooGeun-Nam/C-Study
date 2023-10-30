# **C++**
코드 상단에 함수를 먼저 선언 후 아래에 구현 (호출문제 해결)

#pragma once
header guard

extern 변수 : header 에서 변수 미리 알려주는 장치

게임은 입력, 로직, 출력 의 순서를 가진다




---

# **언리얼 블루프린트**
Name : 언리얼 엔진상에서 변하지 않는 이름
Text : 변하지 않는 것 / UI로 표시되는것 / 다국어 지원 하는 것
String : 바뀔 여지가 있는 것 / 그 외 일반적인 것

---


# **Game Server**
MassivelyMultiPlayerOnline
대규모 다중 사용자

Actor
- Player
- NPC
- ControlZone

서버
1. 패킷처리 (Road/Write)
parsing -> action
2. Tick 처리
매 프레임 Actor의 동작을 업데이트
3. 그외 System

패킷,틱 처리 = Actor 수에 비례한다

100ms 당 틱을 처리한다 하면 100ms 안에 모든 Actor를 업데이트 해야된다.
5000명이라면 0.02ms 에 1개의 Actor를 처리해야된다
==> 쉽지 않기 때문에 멀티 쓰레드, 코어를 나눠서 작업을 분산

Network I/O -> Queue -> WorkThread 구조

Network I/O (멀티스레드)
- Select
- IOCP Server => 멀티스레드 형식
WorkThread 처리 하는곳(싱글이냐 멀티냐 차이)
: 범퍼로 Queue(패킷을 쌓아놓을) 가 필요함 1Tick에 Queue를 다 비워냄

WorkThread 종류
- Single
장점 : 데드락이나 복잡한 문제 X
단점 : 많은 Actor를 처리할 수 없다
- Single + Dedicate (+A)
기존 WorkThread + 경매장(or전투) Thread
특정 기능의 처리만 담당하는 스레드 locking 을 하거나 async 형태로 통신하는 기능이 필요
장점 : 싱글스레드의 단점 보완
- MultiThread
각 각 스레드에서 Queue에서 가져가서 처리, 먼저 가져간걸 먼저 처리
단순히 WorkThread가 여러개
문제점 : 충돌이 자주 일어남
Queue에 Locking 처리 필요
Actor 요청 에도 Locking
웹 서버 : 해당 요청만 핸들링 해서 응답 하면됨
게임 서버 : Actor 간의 Interaction
A가 B를 타격했을때 다른 WorkThread에서 처리 한다면 DeadLock 발생확률

==> 멀티쓰레드의 구획화 필요

멀티프로세스는 DeadLock X (같은 메모리영역을 공유하지 않는다) 하지만 서로 Interaction이 불가능
SeamLess 라면 단일 프로세스


싱글 프로세스 vs 멀티 프로세스
싱글 프로세스 : 게임서버가 하나다
Scale Up : 머신의 성능 증가
머신의 성능의 증가가 프로세스의 성능에 비례하지 않는다

Scale Out : 머신의 수 증가