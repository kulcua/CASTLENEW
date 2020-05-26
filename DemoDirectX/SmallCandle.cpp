#include "SmallCandle.h"



SmallCandle::SmallCandle()
{
}

void SmallCandle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state == 1 && animation_set->at(1)->RenderOver(300))
		isDone = true;
}

void SmallCandle::Render()
{
	if (!isDone)
		animation_set->at(state)->Render(-1, x, y);
	else
		return;

	//RenderBoundingBox();
}

void SmallCandle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state != 1)
	{
		l = x;
		t = y;
		r = l + 16;
		b = t + 32;
	}
}

void SmallCandle::SetState(int State)
{
	CGameObject::SetState(State);
	switch (State)
	{
	case 1:
		animation_set->at(State)->StartAni();
		break;
	}
}


SmallCandle::~SmallCandle()
{
}
