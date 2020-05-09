﻿#include "Knife.h"


Knife::Knife()
{
	isDone = true;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(knife_ani_set));
}

void Knife::collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	SubWeapon::collisionwith(dt, coObjects);
}

void Knife::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	SubWeapon::Update(dt);

	if (CheckPosKnife(POSX))
	{
		isDone = true;
		return;
	}

	collisionwith(dt, coObjects);

}


void Knife::Render()
{
	if (!isDone && !CheckPosKnife(POSX))
		SubWeapon::Render();

	SubWeapon::renderlisthit();

	//RenderBoundingBox();

}



void Knife::SetState(int State)
{
	CGameObject::SetState(State);

	if (nx > 0)
		vx = 0.5;
	else
		vx = -0.5;
	vy = 0;
}

void Knife::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isDone)
	{
		left = x;
		top = y;
		right = left + knife_box_width;
		bottom = top + knife_box_height;
	}

}

void Knife::SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding)
{
	SubWeapon::SetPosSubWeapon(D3DXVECTOR3(pos.x, pos.y, 0), isstanding);
	POSX = pos.x;
}

bool Knife::CheckPosKnife(float a)
{
	if (vx > 0)
	{
		if (x - a >= ((SCREEN_WIDTH / 2) + 100))
			return true;
	}
	else if (vx < 0)
	{
		if (a - x >= ((SCREEN_WIDTH / 2) + 100))
			return true;
	}
	return false;
}
Knife::~Knife()
{
}
