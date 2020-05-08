#pragma once
#include"SubWeapon.h"

#define knife_ani 0
#define knife_ani_set 6

#define SCREEN_WIDTH 530
#define SCREEN_HEIGHT 500
#define knife_box_width 34
#define knife_box_height 18
class Knife :public SubWeapon
{
	float POSX;
public:
	Knife();
	~Knife();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Render();
	void collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void SetState(int SetState);
	void SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding);
	bool CheckPosKnife(float a);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

