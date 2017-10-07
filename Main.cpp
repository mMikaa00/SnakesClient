#include "Game.h"
#include <process.h>

#ifndef NDEBUG
HANDLE hSemaphore,h_event1,h_event2;
unsigned int dwThreadId;
HANDLE hThread;
#endif // !NDEGUG


void main()
{
	/*WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		exit(0);
	}*/

	CONSOLE_CURSOR_INFO mycursor;
	GetConsoleCursorInfo(hout, &mycursor);
	mycursor.bVisible = false;
	SetConsoleCursorInfo(hout, &mycursor);

	Game mygame;
	mygame.newgame();
	mygame.gconnect();
	mygame.login();
	mygame.loaddata();
	system("cls");

#ifndef NDEBUG

	h_event1 = CreateEvent(NULL, true, false, nullptr);
	h_event2 = CreateEvent(NULL, true, false, nullptr);
	hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);//´´½¨ÐÅºÅ
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&mygame, 0, &dwThreadId);
	
#endif // !NDEBUG

	while (true) {
		mygame.paintwindow();
		mygame.set();
		SetEvent(h_event1);
		mygame.playgame();
		if (!mygame.playagain()) {
#ifndef NDEBUG
			SetEvent(h_event2);
			ReleaseSemaphore(hSemaphore, 1, NULL);
			CloseHandle(hSemaphore);
			CloseHandle(hThread);
			CloseHandle(h_event1);
			CloseHandle(h_event2);
#endif // !NDEBUG
			break;
		}
		system("cls");
		mygame.reset();
	};
}