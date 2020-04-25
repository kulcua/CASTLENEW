#pragma once
#include"GameObject.h"
#include"Candle.h"
#include"Gate.h"
#include"Hit.h"
#include"Ground.h"

#define weapon_knfie 0
#define weapon_watch 1
class SubWeapon :public CGameObject
{
	vector<LPHIT> listHit;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Render();

	virtual void SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	virtual void collisionwith(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void renderlisthit();
	Hit* CreateHit(float x, float y);
	SubWeapon();
	~SubWeapon();
};

