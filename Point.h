#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

static bool chekans(const int &sock)
{
	char buff[4];
	int n;
	while ((n = recv(sock, buff, 4, 0)) < 0)//可将此处单独顶一个为一个Recv函数代替原函数
	{
		if (errno == EINTR)
			continue;
		else
			return false;
	}
	buff[n] = 0;
	return (strcmp("LWB", buff) ? false : true);
}

static void SetOutputposition(int x, int y, HANDLE hand)
{
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(hand, position);
}

static int n;

static HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

class Point
{
public:
	Point(int a=0,int b=0):x(a),y(b){}
	void paint(HANDLE hand)
	{
		SetOutputposition(x, y, hand);
		cout << "■";
	}
	void clear(HANDLE hand)
	{
		SetOutputposition(x, y, hand);
		cout << "  ";
	}
	int& xx() {
		return x;
	}
	const int& xx() const
	{
		return x;
	}
	int& yy() {
		return y;
	}
	const int &yy() const
	{
		return y;
	}
	bool operator==(const Point& point) const
	{
		if (x == point.x&&y == point.y)
			return true;
		else
			return false;
	}
private:
	int x;
	int y;

};