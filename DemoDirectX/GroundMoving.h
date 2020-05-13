#pragma once
#include"GameObject.h"
#include"Ground.h"
class GroundMoving :public CGameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	GroundMoving();
	~GroundMoving();
};

