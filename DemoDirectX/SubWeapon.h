#pragma once
#include"GameObject.h"
#include"Candle.h"
#include"Gate.h"
#include"Knight.h"
#include"Monkey.h"
#include"Frog.h"
#include"Hit.h"
#include"Ground.h"

#define weapon_knfie 0
#define weapon_watch 1
class SubWeapon :public CGameObject
{
protected:
	vector<LPHIT> listHit;
	LPGAMEOBJECT simon;
	float POSX;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Render();

	virtual void SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void collisionwith(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void SetV() = 0;
	void renderlisthit();
	Hit* CreateHit(float x, float y);
	virtual void SetState(int State){}
	SubWeapon();
	SubWeapon(LPGAMEOBJECT simon)
	{
		this->simon = simon;
	}
	~SubWeapon();
};

