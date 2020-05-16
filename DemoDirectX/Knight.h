#pragma once
#include"Enemy.h"
#include"Bat.h"
#define knight_box_height 80
#define knight_box_width 40
#define knight_speed 0.08
#define knight_ani_run 0
#define knight_ani_die 1
#define knight_ani_die_time 300
class Knight :public Enemy
{
	float maxX1, maxX2;
public:
	Knight(float maxX1, float maxX2);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	~Knight();
};

