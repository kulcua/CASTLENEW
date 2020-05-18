#pragma once
#include"Enemy.h"
#include"Game.h"
class Monkey :public Enemy
{
	LPGAMEOBJECT simon;
public:
	Monkey(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	bool CheckCam();
	~Monkey();
};

