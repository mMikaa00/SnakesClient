#include "Snake.h"


void Snake::paintbody(HANDLE hand)
{
	for (auto &k : body)
	{
		k.paint(hand);
	}
}

bool Snake::recvdata(int &sock)
{
	char recvline[200];
	send(sock, "B", 1, 0);
	if ((n = recv(sock, recvline, 200, 0)) > 0)
	{
		recvline[n] = 0;
		body.clear();
		for (int i = 1; i < n; i++, i++)
		{
			Point temp;
			temp.xx() = recvline[i];
			temp.yy() = recvline[i + 1];
			body.push_back(temp);
		}
	}
	else
		return false;
	send(sock, "D", 1, 0);
	if ((n = recv(sock, recvline, 200, 0)) > 0) {
		recvline[n] = 0;
		direction = recvline[1];
	}
	else
		return false;
	return true;
}

void Snake::reset()
{
	body = { { 20,20 },{ 20,21 },{ 20,22 } };
	direction = 'd';
	bodysize = body.size();
}





