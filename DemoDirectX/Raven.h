#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Timer.h"
#define raven_ani_idle 0
#define raven_ani_fly 1
#define raven_ani_die 2
#define raven_time 300
class Raven :public Enemy
{
	LPGAMEOBJECT simon;

	Timer* wait = new Timer(1800);
	Timer* wait1 = new Timer(1500);
	Timer* dow = new Timer(1500);
	Timer* dow1 = new Timer(1500);
	Timer* timerstopdown = new Timer(2000);
	bool checkstar,timeflydown;
public:
	Raven(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	bool CheckCam();
	~Raven();
};

