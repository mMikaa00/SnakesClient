#include "Game.h"

#ifndef NDEBUG
extern HANDLE hSemaphore,h_event1,h_event2;
#endif // !NDEBUG

Game::Game()
{
	speed = 300;
	score = 0;
	record = 0;
	//recordfile.open("C:\\Users\\Administrator\\Desktop\\c\\Snakes\\record.txt", fstream::in | fstream::out);
	//recordfile.seekg(0, fstream::beg);    //读取记录
	//recordfile >> record;
}

Game::~Game()
{
	//recordfile.close();
	closesocket(sockfd);
	WSACleanup();
}

void Game::newgame() {
	//SNAKES LOGO
	SetConsoleTextAttribute(hout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	SetOutputposition(34, 6, hout);
	cout << " ■■■   ■     ■  ■■■  ■    ■ ■■■■";
	SetConsoleTextAttribute(hout, FOREGROUND_GREEN);
	SetOutputposition(34, 7, hout);
	cout << "■    ■  ■■   ■ ■    ■ ■   ■  ■";
	SetOutputposition(34, 8, hout);
	SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "■        ■ ■  ■ ■    ■ ■  ■   ■";
	SetOutputposition(34, 9, hout);
	SetConsoleTextAttribute(hout, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << " ■■■   ■  ■ ■ ■■■■ ■■     ■■■";
	SetOutputposition(34, 10, hout);
	SetConsoleTextAttribute(hout, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "      ■  ■   ■■ ■    ■ ■  ■   ■";
	SetOutputposition(34, 11, hout);
	SetConsoleTextAttribute(hout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "■    ■  ■     ■ ■    ■ ■   ■  ■";
	SetOutputposition(34, 12, hout);
	SetConsoleTextAttribute(hout, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << " ■■■   ■     ■ ■    ■ ■    ■ ■■■■";
	int count = 6;
	int interval = 600;
	while (true) {
		//闪动字
		SetOutputposition(45, 24, hout);
		cout << "press any key to start...";
		Sleep(interval);
		for (int i = 0; i != 25; i++)
			cout << "\b \b";
		if (_kbhit()) {
			if (count-- == 0)
				break;
			interval = 50;
		}
		Sleep(interval);
		if (_kbhit()) {
			if (count-- == 0)
				break;
			interval = 50;
		}
	}
	while (_kbhit()) //清除输入流中的数据
		_getch();
}

void Game::gconnect()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		exit(0);
	}

	struct sockaddr_in servaddr;
	//if(argc!=2)
	//	err_quit("usage:a.out <IPaddress>");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	inet_pton(AF_INET, "192.168.199.244", &servaddr.sin_addr);

	while (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) {
		SetOutputposition(38, 24, hout);
		printf("connect error");
		Sleep(1000);
		printf("   press any key to retry");
		_getch();
		for (int i = 0; i != 40; i++)
			cout << "\b \b";
	}
	SetOutputposition(53, 24, hout);
	printf("connected!");
	Sleep(1000);
	for (int i = 0; i != 11; i++)
		cout << "\b \b";
}

void Game::login()
{
	while (1) {
		char sendline[20];
		SetOutputposition(38, 24, hout);
		cout << "Username: ";
		sendline[0] = 'u';
		/*while (_kbhit())
		{
			char df = _getch();
			if (isalnum(df))
				break;
		}*/
		cin >> (sendline + 1);
		send(sockfd, sendline, strlen(sendline), 0);
		SetOutputposition(38, 25, hout);
		cout << "Password: ";
		for (int i = 0; i != 18;)
		{
			char temp = _getch();
			if (isalnum(temp))
			{
				sendline[i] = temp;
				cout << '*';
				i++;
			}
			else if (temp == '\b'&&i > 0) {
				i--;
				sendline[i] = '\0';
				cout << "\b \b";
			}
			else if (temp == '\r') {
				if (i == 0)
					continue;
				sendline[i] = '\0';
				break;
			}

		}
		send(sockfd, sendline, strlen(sendline), 0);
		char recvmsg[4];
		int n = recv(sockfd, recvmsg, 4, 0);
		recvmsg[n] = 0;
		SetOutputposition(38, 26, hout);
		if (recvmsg[0] == 'y') {
			//cout << "User information confirmed!" << endl;
			break;
		}
		cout << "User informaton error,please try again!";
		//for (int i = 0; i != 40; i++)
		//cout << "\b \b";
		SetOutputposition(78, 24, hout);
		for (int i = 0; i != 40; i++)
			cout << "\b \b";
		SetOutputposition(78, 25, hout);
		for (int i = 0; i != 40; i++)
			cout << "\b \b";
	}
}

void Game::loaddata()
{
	snake.recvdata(sockfd);
	food.recvdata(sockfd);
	char recvline[3];
	send(sockfd, "S", 1, 0);
	if ((n = recv(sockfd, recvline, 3, 0)) > 0)
		recvline[n] = 0;
	score = recvline[1];
	send(sockfd, "R", 1, 0);
	if ((n = recv(sockfd, recvline, 3, 0)) > 0)
		recvline[n] = 0;
	record = recvline[1];
}

void Game::paintwindow()
{
	for (int i = 4; i != 100; ++i, ++i)
	{
		SetOutputposition(i, 3, hout);
		cout << "■";
	}
	for (int i = 4; i != 100; ++i, ++i)
	{
		SetOutputposition(i, 30, hout);
		cout << "■";
	}
	for (int i = 3; i != 31; ++i)
	{
		SetOutputposition(2, i, hout);
		cout << "■";
	}
	for (int i = 3; i != 31; ++i)
	{
		SetOutputposition(100, i, hout);
		cout << "■";
	}
}

void Game::set() {
	SetOutputposition(20, 20, hout);
	cout << "please choose difficulty:";
	SetOutputposition(50, 21, hout);
	cout << "1:easy" << endl;
	SetOutputposition(50, 22, hout);
	cout << "2:normal" << endl;
	SetOutputposition(50, 23, hout);
	cout << "3:hard";
choose:
	char k = _getch();
	switch (k)
	{
	case '1':
		level = 1;
		speed = 300;
		break;
	case '2':
		level = 2;
		speed = 250;
		break;
	case '3':
		level = 3;
		speed = 200;
		break;
	default:
		goto choose;
	}
	SetOutputposition(45, 20, hout);
	for (int i = 0; i != 25; i++)
		cout << "\b \b";
	SetOutputposition(60, 21, hout);
	for (int i = 0; i != 10; i++)
		cout << "\b \b";
	SetOutputposition(60, 22, hout);
	for (int i = 0; i != 10; i++)
		cout << "\b \b";
	SetOutputposition(60, 23, hout);
	for (int i = 0; i != 10; i++)
		cout << "\b \b";
}

void Game::playgame() {
	food.foodpaint(hout);      //绘制食物  
	snake.paintbody(hout);     //绘制蛇身
	showstate();
	int speedr(speed);
	while (true) {
		WaitForSingleObject(h_event1, INFINITE);
		ResetEvent(h_event1);
		char k;
		speed = speedr;
		while (_kbhit()) {
			k = _getch();
			if (k == '\r')
				system("pause>>nul");
			if(snake.changedirection(k))
				break;
			if (snake.press(k)) {
				speed = 50;
			}
		}
		snake.update();         //更新蛇出现的位置
		if (hit())             //判断死亡		
			break;
		if (!eaten()) {
			snake.tailclear(hout);
			snake.painthead(hout);
		}
		else
		{
			if (++score%10==0) {
				speed = speedr;
				accelerate();
				speedr = speed;
			}
			showstate();
			food.update();
			while (snake.overlap(food.foodposition()))
				food.update();
			food.foodpaint(hout);
			
		}
#ifdef NDEBUG
		senddata();
#endif // NDEBUG
		SetEvent(h_event2);
		Sleep(speed);
	}
}

bool Game::playagain()
{
	if (score > record) {
		record = score;
		//recordfile.seekp(0, fstream::beg);
		//recordfile << score << endl;
		SetOutputposition(45, 14, hout);
		cout << "NEW RECORD!!!" << endl;
	}
	SetOutputposition(45, 17, hout);
	cout << "GAME OVER!" << endl;
	SetOutputposition(45, 18, hout);
	cout << "play again ? Y or N" << endl;
	while (1) {
		char k = _getch();
		if (k == 'Y' || k == 'y')
			return true;
		else if (k == 'N' || k == 'n')
			return false;
	}
}

void Game::reset()
{
	snake.reset();
	food.reset();
	score = 0;
}

#ifndef NDEBUG
unsigned int WINAPI ThreadFunc(LPVOID lpParam)
{
	DWORD waitresult;
	Game* temp = (Game*)lpParam;
	while (1) {
		if ((waitresult = WaitForSingleObject(hSemaphore, 0))==WAIT_OBJECT_0)
			break;
		WaitForSingleObject(h_event2, INFINITE);
		ResetEvent(h_event2);
		temp->senddata();
		SetEvent(h_event1);
		Sleep(100);
	}
	return 0;
}
#endif // !NDEBUG