#include "Axe.h"

Axe::Axe()
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(20));
	isDone = true;
	isFire = false;
}
void Axe::collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	SubWeapon::collisionwith(dt, coObjects);
}
void Axe::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	SubWeapon::Update(dt);
	SubWeapon::ClearListHit();

	vy += 0.001*dt;


	if (CheckPosKnife(POSX))
	{
		isFire = false;
		isDone = true;
		return;
	}

	collisionwith(dt, coObjects);
}

void Axe::Render()
{
	if (!isDone && !CheckPosKnife(POSX))
		SubWeapon::Render();

	SubWeapon::renderlisthit();

	//RenderBoundingBox();
}

void Axe::SetV()
{
	if (nx > 0)
		vx = 0.2;
	else
		vx = -0.2;
	vy = -0.45;
}

void Axe::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isDone)
	{
		left = x;
		top = y;
		right = left + 30;
		bottom = top + 28;
	}

}
void Axe::SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding)
{
	SubWeapon::SetPosSubWeapon(D3DXVECTOR3(pos.x, pos.y, 0), isstanding);
	POSX = pos.x;
}

bool Axe::CheckPosKnife(float a)
{
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
Axe::~Axe()
{
}
