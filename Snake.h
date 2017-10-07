#pragma once
#include <deque>
#include "Point.h"
#include <conio.h>

class Snake
{
public:
	Snake(initializer_list<Point> li = { {20,20},{20,21},{20,22} }, char c = 'd') :body(li), direction(c) { bodysize = body.size(); };
	void paintbody(HANDLE);
	bool hititself()
	{
		Point head = body.front();
		for (auto k = --body.end(); k != (body.begin() + 2); k--)
			if (head == *k)
				return true;
		return false;
	}
	bool overlap(const Point &p)//�ж�ˢ�µ�ʳ���Ƿ��������غ�
	{
		for (auto &k :body)
			if (p == k)
				return true;
		return false;
	}

	bool senddata(int &sock);

	bool recvdata(int &sock);

	void painthead(HANDLE hand)
	{
		body.front().paint(hand);
	}

	void tailclear(HANDLE hand)
	{
		body.back().clear(hand);
		body.pop_back();
	}

	const Point& Head()
	{
		return body.front();
	}

	bool changedirection(char &k);

	void update();

	void reset();

	bool press(char &k)
	{
		return k == direction;
	}
private:
	deque<Point> body;
	char direction;
	size_t bodysize;
};

inline
bool Snake::senddata(int &sock)
{
	string temp("b");//body������b��ͷ
	int n;
	for (auto k : body) {//*******�˴�k��Ӧʹ�����ã������߳��ж�βԪ�ؽ���ɾ��ʱ����ʹ�����û�ʹ������ʧЧ
		temp.push_back(k.xx());
		temp.push_back(k.yy());
	}
	if ((n = send(sock, temp.c_str(), strlen(temp.c_str()), 0)) < 0 || !chekans(sock))
		return false;
	temp.assign("d");//dir������d��ͷ
	temp.push_back(direction);
	if ((n = send(sock, temp.c_str(), strlen(temp.c_str()), 0)) < 0 || !chekans(sock))
		return false;
	return true;
}

inline
bool Snake::changedirection(char &k)
{
	if (k == 'A' || k == 'S' || k == 'W' || k == 'D')
		k = tolower(k);
	if (k == 'a' || k == 's' || k == 'w' || k == 'd') {
		if ((k + direction) == 234 || (k + direction) == 197 || k == direction)
			return false;
		direction = k;
		return true;
	}
	return false;
}

inline
void Snake::update()
{
	switch (direction) {
	case 'w':
		body.emplace_front(body.front().xx(), body.front().yy() - 1);
		break;
	case 's':
		body.emplace_front(body.front().xx(), body.front().yy() + 1);
		break;
	case 'a':
		body.emplace_front(body.front().xx() - 2, body.front().yy());
		break;
	case 'd':
		body.emplace_front(body.front().xx() + 2, body.front().yy());
		break;
	}
}
