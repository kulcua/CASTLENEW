#pragma once
#include"GameObject.h"
#define time_items_des 3000
#define items_for_whip 0
#define items_big_heart 1
#define items_knife 2
#define items_watch 3
#define items_axe 4
#define items_boom 5
#define items_holywater 6
#define items_watterbottle 7
#define items_corss 8
#define items_double 9
#define items_triple 10
#define items_meat 11
#define items_small_heart 12
#define items_bluemoney 13
#define items_redmoney 14
#define items_whitemoney 15

#define items_ani_set 5


#define items_whip_box_width 32
#define items_whip_box_height 32
#define items_bigheart_box_width 24
#define items_bigheart_box_height 20
#define items_knife_box_width 32
#define items_knife_box_height 18
#define items_watch_box_width 30
#define items_watch_box_height 32
class Items :public CGameObject
{
	DWORD timeStart;
	

public:
	
	Items();
	~Items();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Render();
	void SetState(int SetState);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);

};

