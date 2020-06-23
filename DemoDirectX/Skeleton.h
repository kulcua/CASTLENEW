#pragma once
#include"Enemy.h"
#include"Game.h"
#include"Bone.h"
#include"Ground.h"
#include"Utils.h"
#define skeleton_ani_idle 0
#define skeleton_ani_die 1
#define skeleton_time 300
#define skeleton_box_height 62
#define skeleton_box_width 32
#define skeleton_hp 1
#define skeleton_damage 3
#define skeleton_score 300

#define skeleton_gra 0.001
#define skeleton_vx 0.13
#define skeleton_vy -0.4

#define active_max_dis 270
#define small_dis_one 200
#define small_dis_two 70
#define max_y_jump 250
class Skeleton :public Enemy
{
	LPGAMEOBJECT simon;
	Bone* bone;
	bool jump,nhay1lan,active;


public:
	Skeleton(LPGAMEOBJECT simon);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL,bool clk=false);
	void Render();
	void SetState(int State);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void loseHp(int x);
	int getHp();
	
	Bone* GetBone() { return bone; }
	bool CheckCam();
	~Skeleton();
};

