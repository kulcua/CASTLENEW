#pragma once
#include"GameObject.h"
class Enemy :public CGameObject
{
protected:
	int hp;
	int damage;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render() = 0;
	virtual void SetState(int State);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b) = 0;
	virtual void loseHp(int x);
	virtual int getHp();
	Enemy();
	~Enemy();
};

