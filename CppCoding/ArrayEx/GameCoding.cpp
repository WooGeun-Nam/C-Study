// GameCoding.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
using namespace std;

#include "Helper.h"
#include "Map.h"
#include "Player.h"

int main()
{
	SetCursorOnOff(false);

	// MovePlayer(3, 2);

	while (true)
	{
		// 입력
		HandleKeyInput();

		// 로직
		HandleMove();

		// 출력
		PrintMap2D();
	}
}