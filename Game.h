#pragma once
#include "TCPhead.h"
#include "Food.h"
#include "Snake.h"
#include <fstream>
#include <string>
//#define NDEBUG

using namespace std;
class  Game
{
public:
	Game();
	~Game();
	void newgame();
	void gconnect();
	void login();
	void loaddata();
	void paintwindow();
	void set();
	void playgame();
	bool playagain();
	void reset();
	void accelerate()
	{
		speed -= speed / 5;
		level++;
	}

	bool eaten()
	{
		return (snake.Head() == food.foodposition());
	}
	bool hit()
	{
		//×²Ç½
		int x, y;
		x = snake.Head().xx();
		y = snake.Head().yy();
		if (x == 2 || x == 100)
			return true;
		if (y == 3 || y == 30)
			return true;
		//×²×Ô¼º
		return snake.hititself();
	}
	//×´Ì¬À¸
	void showstate()
	{
		SetOutputposition(2, 2, hout);
		cout << "Difficulty: " << level;
		SetOutputposition(48, 2, hout);
		cout << "Scores: " << score;
		SetOutputposition(88, 2, hout);
		cout << "Record: " << record;
	}
	bool senddata()
	{
		string temp("s");
		temp.push_back(score);
		if ((n = send(sockfd, temp.c_str(), strlen(temp.c_str()), 0)) < 0 || !chekans(sockfd))
			return false;
		temp.assign("r");
		temp.push_back(record);
		if ((n = send(sockfd, temp.c_str(), strlen(temp.c_str()), 0)) < 0 || !chekans(sockfd))
			return false;
		if(!snake.senddata(sockfd)||!food.sendfood(sockfd))
			return false;
		return true;
	}	
private:
	int speed;
	short level;
	Snake snake;
	Food food;
	unsigned short score;
	unsigned short record;
	//fstream recordfile;
	int sockfd;
};

unsigned int WINAPI ThreadFunc(LPVOID lpParam);
