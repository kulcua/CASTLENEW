#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Timer.h"
#define frog_ani_die 1
#define frog_time 300
class Frog :public Enemy
{
	LPGAMEOBJECT simon;
	bool check;
	
	
public:
	DWORD timestop;
	Frog(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL,bool clk=false);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	bool CheckCam();
	~Frog();
};

