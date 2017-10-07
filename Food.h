#pragma once
#include "Point.h"

class Food
{
public:
	Food(int a = 14, int b = 15) :food{ a,b } {}
	void foodpaint(HANDLE hand)
	{
		food.paint(hand);
	}
	const Point& foodposition()
	{
		return food;
	}

	void update()
	{
		food.xx() = 4+rand() % 48*2;
		food.yy() =  4 + rand() % 26;
	}
	bool sendfood(const int &sock)
	{
		string temp("f");//food数据以f开头
		temp.push_back(food.xx());
		temp.push_back(food.yy());
		if ((n = send(sock, temp.c_str(), strlen(temp.c_str()), 0)) < 0 || !chekans(sock))
			return false;
		else
			return true;
	}

	bool recvdata(int &sock)
	{
		char recvline[4];
		send(sock, "F", 1, 0);
		if ((n = recv(sock, recvline, 4, 0)) > 0) {
			recvline[n] = 0;
			food.xx() = recvline[1];
			food.yy() = recvline[2];
			return true;
		}
		else
			return false;
		
	}
	void reset()
	{
		food = { 14,15 };
	}
private:
	Point food;
};

