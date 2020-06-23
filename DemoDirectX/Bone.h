#pragma once
#include"Enemy.h"
#include"Game.h"
#define bone_hp 3
#define bone_gravity 0.001
#define dis_add 100
#define bone_vx 0.15
#define bone_vy -0.45

#define bone_aniset 27

#define bone_box_width 32
#define bone_box_height 32

#define add_dis 5

#define bone_ani_fly 0

class Bone :public Enemy
{
	LPGAMEOBJECT s;
	bool checkset;
	float POSX;
public:
	Bone(LPGAMEOBJECT s);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void setpos(D3DXVECTOR2 pos);
	bool CheckCam();
	void setcheck(bool a) { checkset = a; }
	bool getcheck() { return checkset; }
	bool checkposX(float a) {
		if (vx > 0)
		{
			if (x - a >= ((SCREEN_WIDTH / 2) + dis_add))
				return true;
		}
		else if (vx < 0)
		{
			if (a - x >= ((SCREEN_WIDTH / 2) + dis_add))
				return true;
		}
		return false;
	}
	~Bone();
};

