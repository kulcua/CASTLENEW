#pragma once
#include"Enemy.h"
#include"Ground.h"
#include"Bat.h"
#define knight_box_height 64
#define knight_box_width 32
#define knight_speed 0.08
#define knight_ani_run 0
#define knight_ani_die 1
#define knight_ani_die_time 300
class Knight :public Enemy
{
	bool back;
	
public:
	DWORD timestop;
	Knight(/*float maxX1, float maxX2*/);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL,bool clk=false);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	~Knight();
};

