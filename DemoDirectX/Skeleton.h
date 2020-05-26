#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Bone.h"
#include"Ground.h"
#include"Utils.h"
#define skeleton_ani_die 1
#define skeleton_time 300

class Skeleton :public Enemy
{
	LPGAMEOBJECT simon;
	Bone* bone;
	bool jump,nhay1lan,active;


public:
	Skeleton(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	
	Bone* GetBone() { return bone; }
	bool CheckCam();
	~Skeleton();
};

