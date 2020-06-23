#include "SmallCandle.h"



SmallCandle::SmallCandle()
{
}

void SmallCandle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects,bool clk)
{
	if (state == break_candle && animation_set->at(break_candle)->RenderOver(smallcandle_time))
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
	if (state != break_candle)
	{
		l = x;
		t = y;
		r = l + smallcandle_box_width;
		b = t + smallcandle_box_height;
	}
}

void SmallCandle::SetState(int State)
{
	CGameObject::SetState(State);
	switch (State)
	{
	case break_candle:
		animation_set->at(State)->StartAni();
		break;
	}
}


SmallCandle::~SmallCandle()
{
}
