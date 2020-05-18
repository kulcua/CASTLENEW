#pragma once

#include "GameObject.h"
#include "Whip.h"
#include "Items.h"
#include "SubWeapon.h"
#include "GroundMoving.h"
#include "Knife.h"
#include "Gate.h"
#include "Stair.h"
#include "Knight.h"
#include "Bat.h"
#include "Monkey.h"
#include "Timer.h"

#define simon_jump -0.5f
#define simon_run 0.15f
#define simon_gravity 0.0015f
#define max_screen_left -11
#define simon_ani_idle 0
#define simon_ani_run  1
#define simon_ani_jump 2
#define simon_ani_sit  3
#define simon_ani_stand_hit 4
#define simon_ani_sit_hit   5
#define simon_ani_led 6
#define simon_ani_stair_up 7
#define simon_ani_stair_down 8
#define simon_ani_stair_up_hit 9
#define simon_ani_stair_down_hit 10
#define simon_ani_hurt 11
#define simon_ani_dead 12

#define simon_stair 0.079

#define simon_ani_set 1

#define simon_max_health 16

#define simon_box_width 30
#define simon_box_height 62

#define simon_delay_hit 300
#define simon_delay_led 600
class Simon :public CGameObject
{
	Whip *whip;

	SubWeapon *knife;

	int health;
	int score;
	int mana;
	int life;

	
public:

	Timer* untouchtime = new Timer(1200);
	Timer* watertime = new Timer(4000);
	bool checkgroundmove=false;
	bool isCross;
	bool isDead;

	int currentscene;
	int beforescene;

	float newPosX;
	int stateAfterAutoWalk = -1;	
	int nxAfterAutoWalk;

	bool StairLeftFirst;

	bool isWalkStair;
	bool isStandOnStair;
	bool canmoveupstair;
	bool canmovedownstair;
	int stairNx;

	LPGAMEOBJECT stairCollided;

	bool isGrounded;
	bool isHitSubWeapon;
	bool isChangeScene;
	int currentWeapon;
	Simon();
	~Simon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Render();
	void SetState(int SetState);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	Whip* GetWhip()
	{
		return whip;
	}
	SubWeapon *GetKnife()
	{
		return knife;
	}
	int getcurrentweapon() { return currentWeapon; }
	int getscore() { return score; }
	int getmana() { return mana; }
	int getlife() { return life; }
	void usemana(int a) { mana -= a; }
	void SimonColliWithItems(vector<LPGAMEOBJECT> *listitems);
	bool SimonColliWithStair(vector<LPGAMEOBJECT> *liststair);
	void StandOnStair();
	void AutoWalkStair(float newPosX, int stateAfterAutoWalk, int nxAfterAutoWalk);
	void DoAutoWalkStair();
	void loseHp(int x);
	void setHealth(int a) { health = a; }
	void setMana(int a) { mana = a; }
	void setScore(int a) { score = a;}
	void setLife(int a) { life = a; }
	void addScore(int a) { score += a; }
	int GetHealth() { return health; }
};

