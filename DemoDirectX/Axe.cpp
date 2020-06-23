#include "Axe.h"

Axe::Axe()
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(axe_ani_set));
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

	vy += axe_gra *dt;


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
		vx = axe_vx;
	else
		vx = -axe_vx;
	vy = axe_vy;
}

void Axe::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isDone)
	{
		left = x;
		top = y;
		right = left + axe_box_width;
		bottom = top + axe_box_height;
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
		if (x - a >= ((SCREEN_WIDTH / 2) + axe_max_dis))
			return true;
	}
	else if (vx < 0)
	{
		if (a - x >= ((SCREEN_WIDTH / 2) + axe_max_dis))
			return true;
	}
	return false;
}
Axe::~Axe()
{
}
