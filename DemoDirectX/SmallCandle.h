#pragma once
#include"GameObject.h"
#define break_candle 1
#define smallcandle_time 300
class SmallCandle :public CGameObject
{
public:
	SmallCandle();
	~SmallCandle();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

