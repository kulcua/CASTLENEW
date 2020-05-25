#pragma once
#include"GameObject.h"
class BreakWall :public CGameObject
{
public:
	
	virtual void Render();
	/*void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
	{
		if (check)
			isDone = true;
	}*/
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	BreakWall();
	~BreakWall();
};

