#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Timer.h"
#define frog_ani_die 1
class Frog :public Enemy
{
	LPGAMEOBJECT simon;
	bool check;
	
	/*Timer* wait = new Timer(2800);
	Timer* wait1 = new Timer(2500);
	Timer* dow = new Timer(2500);
	Timer* dow1 = new Timer(2500);
	Timer* asd = new Timer(4000);
	bool checkstar,qwe;*/
public:
	Frog(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	bool CheckCam();
	~Frog();
};

