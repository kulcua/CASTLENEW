#pragma once
#include"Enemy.h"
#include"Game.h"

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
			if (x - a >= ((530 / 2) + 100))
				return true;
		}
		else if (vx < 0)
		{
			if (a - x >= ((530 / 2) + 100))
				return true;
		}
		return false;
	}
	~Bone();
};

