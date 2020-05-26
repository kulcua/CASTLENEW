#pragma once
#include"GameObject.h"
#include"Hit.h"
#include"Knight.h"
#include"Bat.h"
#include"Skeleton.h"
#include"Raven.h"
#include"Monkey.h"
#include"BreakWall.h"
#include"SmallCandle.h"
#include"Frog.h"
#define whip_lv1 0
#define whip_lv2 1
#define whip_lv3 2

#define whip_ani_set 4

#define whip_box_width_lv12 55
#define whip_box_width_lv3 85
#define whip_box_height 15

class Whip :public CGameObject
{
	vector<LPHIT> listHit;
	bool delaydamage;
	//int score;
public:
	Whip();
	~Whip();
	Hit* CreateHit(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Render(int currentID = -1);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetPosWhip(D3DXVECTOR3 pos, bool isstanding);
	void Setdelaydamage(bool a) { delaydamage = a; }
	bool WhipCheckColli(float l_b, float t_b, float r_b, float b_b);
	void SetLvWhip(int state)
	{
		this->state = state;
	}
	/*int getScore() {
		return score;
	}*/
	//void setScore(int a) { score = a; }

};

