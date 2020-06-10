#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Frog.h"
#include"Ground.h"
#define monkey_ani_run 0
#define monkey_ani_die 1
#define monkey_time 300
class Monkey :public Enemy
{
	LPGAMEOBJECT simon;
	//bool jump;
public:
	Monkey(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL,bool clk=false);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	bool CheckCam();
	~Monkey();
};

