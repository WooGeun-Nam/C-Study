# **C++**
코드 상단에 함수를 먼저 선언 후 아래에 구현 (호출문제 해결)

### header file
#pragma once
header guard

extern 변수 : header 에서 변수 미리 알려주는 장치

### Pointer
핵심은 성능 차이를 유발한다
[타입]* [이름];
- * = 포인터
- 주소값을 담는 바구니
- 주소값을 타고 가면 -> 무엇이 있느냐?

포인터 연산
- 주소 연산자 (&)
- 산술 연산자 (+-)
- 간접 연산자 (*)
- 간접 멤버 연산자 (->)

포인터를 사용하지 않고 함수에서 인자로 가져오게 되면 값을 복사하여 전달 받는다
포인터를 사용하게 되면 주소를 전달 받게 된다.
=> 구조체의 커지가 커지면 커질수록 복사되는 양이 늘어난다.
1) 원본을 건드리고 싶을 때 (원격)
2) 복사 비용 X
3) nullptr; 를 표현할 수 있다 (존재하지 않는 상황을 판별할 수 있다)
-> 처리하지 않았을 때 오류가 발생할 수 있다
```
int numbers[100] = {1,2,3,4,5,6}
int* ptr = numbers;
*(ptr + 3) = numbers[3];
// ===
struct Str
{
    int data;
}

void ByPointer(Str* strt)
{
    cout << strt->data;
}

Str strt1;
strt1.data = 10;
Str* ptr = &strt1;
(*ptr).data <=> ptr->data

ByPointer(&strt1);

```

### Reference
원본에 별칭을 붙힌다
ref 가 주소를 가지고 있으며 원본을 건드린다.
내부 원리는 pointer 지만 기본문법으로 사용가능
- 비어있는 참조를 만들 수 없다 (없는 상황을 가정할 수 없다)
```
// 키워드를 통해서 변경이 되는걸 명시하는게 좋다
#define OUT
함수(OUT 구조체& 변수명)
{
}

함수(const 구조체& 변수명)
{
    // 원본을 건드리지 않게 하기 위해 의사전달
}
```

### 객체지향 (Object Oriented Programming)
모든것을 객체 단위로 생각하겠다
데이터와 행동 모든것이 객체 (데이터 + 로직)
생성자(클래스명()) , 소멸자(~클래스명())

struct vs class
struct : 여러가지 데이터를 모아서 관리하는 용도
class : 객체지향 용도

부모 class 소멸자에 virtual 안 붙히면 어떻게 되는가??
: 가상 함수로 정의되지 않은 자식 클래스의 함수를 호출하면 부모 클래스의 멤버 함수가 호출된다.
소멸자도 자식 클래스의 소멸자가아닌 부모클래스의 소멸자가 호출이된다.
-> 가상함수 사용 시 자식 클래스의 소멸자부터 차례대로 부모 클래스의 소멸자가 호출된다.
```
// 설계도
class Knight
{
    public:
        // 멤버 함수
        void 함수() {}
    public:
        // 멤버 변수
        int 변수;
};
```

### 동적할당
- 스택 : 위험한 영역 (함수가 날아가면 유지가 안됨)
- 메모리
- 힙 : 안전한 공간 (직접적으로 관리를 해야한다)
실시간으로 요청을 받아서 메모리를 할당
운영체제에 메모리를 요청
- malloc + free(메모리 반납)
  malloc(요청받을 메모리 크기);
  void* ptr = malloc(1000)
  free(ptr); -> 하지않는다면 crash 날 수 있다

- cpp : new + delete
  new, malloc의 차이 : 생성자 소멸자 호출의 차이
  ```
  Class* c1 = new Class(); // 배열도 가능 Class[]()
  c1->var = 100;
  delete c1; // 배열삭제시 delete[]
  c1 = nullptr; // crash 유도

  // Use-After-Free
  c1->var = 200;
  ```


---

# **Game Server**
MassivelyMultiPlayerOnline
대규모 다중 사용자

게임은 입력, 로직, 출력 의 순서를 가진다

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

---

# **언리얼 블루프린트**
Name : 언리얼 엔진상에서 변하지 않는 이름
Text : 변하지 않는 것 / UI로 표시되는것 / 다국어 지원 하는 것
String : 바뀔 여지가 있는 것 / 그 외 일반적인 것