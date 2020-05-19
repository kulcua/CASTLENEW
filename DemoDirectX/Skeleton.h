#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Bone.h"
class Skeleton :public Enemy
{
	LPGAMEOBJECT simon;
	Bone* bone;
	bool check;

public:
	Skeleton(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	

	bool CheckCam();
	~Skeleton();
};

