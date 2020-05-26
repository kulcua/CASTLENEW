#include "Items.h"
Items::Items()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(items_ani_set));

	timeStart = -1;	
	isDone = false;
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	if (state == items_small_heart&&vy!=0)
	{
		vx += 0.005;
		if (vx >= 0.1|| vx <= -0.1)
			vx *= -1;
	}


	if (timeStart == -1)
		timeStart = GetTickCount();
	else
	{
		if (GetTickCount() - timeStart > time_items_des)
		{
			isDone = true;
			return;
		}
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		y += min_ty * dy + ny * 0.1f;

		if (ny != 0)
			vx = vy = 0;
		
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Items::Render()
{
	if (isDone)
		return;

	animation_set->at(state)->Render(-1, x, y);
	//RenderBoundingBox();
}

void Items::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isDone)
	{
		switch (state)
		{
		case items_for_whip:
			l = x;
			t = y;
			r = l + items_whip_box_width;
			b = t + items_whip_box_height;
			break;
		case items_big_heart:
			l = x;
			t = y;
			r = l + items_bigheart_box_width;
			b = t + items_bigheart_box_height;
			break;
		case items_knife:
			l = x;
			t = y;
			r = l + items_knife_box_width;
			b = t + items_knife_box_height;
			break;
		case items_watch:
			l = x;
			t = y;
			r = l + items_watch_box_width;
			b = t + items_watch_box_height;
			break;
		case items_axe:
			l = x;
			t = y;
			r = l + 30;
			b = t + 28;
			break;
		case items_boom:
			l = x;
			t = y;
			r = l + 30;
			b = t + 28;
			break;
		case items_holywater:
			l = x;
			t = y;
			r = l + 32;
			b = t + 28;
			break;
		case items_watterbottle:
			l = x;
			t = y;
			r = l + 29;
			b = t + 33;
			break;
		case items_corss:
			l = x;
			t = y;
			r = l + 32;
			b = t + 32;
			break;
		case items_double:
			l = x;
			t = y;
			r = l + 28;
			b = t + 28;
			break;
		case items_triple:
			l = x;
			t = y;
			r = l + 28;
			b = t + 28;
			break;
		case items_meat:
			l = x;
			t = y;
			r = l + 32;
			b = t + 26;
			break;
		case items_small_heart:
			l = x;
			t = y;
			r = l + 16;
			b = t + 16;
			break;
		case items_redmoney:
		case items_bluemoney:
		case items_whitemoney:
			l = x;
			t = y;
			r = l + 30;
			b = t + 30;
			break;
		}
	}
}

void Items::SetState(int State)
{
	CGameObject::SetState(State);
	switch (State)
	{
	case items_for_whip:
		vx = 0;
		vy = 0.15;
		break;
	case items_big_heart:
		vx = 0;
		vy = 0.15;
		break;
	case items_knife:
		vx = 0;
		vy = 0.15;
		break;
	case items_watch:
		vx = 0;
		vy = 0.15;
		break;
	case items_axe:
		vx = 0;
		vy = 0.15;
		break;
	case items_boom:
		vx = 0;
		vy = 0.15;
		break;
	case items_holywater:
		vx = 0;
		vy = 0.15;
		break;
	case items_watterbottle:
		vx = 0;
		vy = 0.15;
		break;
	case items_corss:
		vx = 0;
		vy = 0.15;
		break;
	case items_double:
		vx = 0;
		vy = 0.15;
		break;
	case items_triple:
		vx = 0;
		vy = 0.15;
		break;
	case items_meat:
		vx = 0;
		vy = 0.15;
		break;
	case items_small_heart:
		vx = 0;
		vy = 0.05;
		break;
	case items_redmoney:
	case items_bluemoney:
	case items_whitemoney:
		vx = 0;
		vy = 0.15;
		break;
	}
}

Items::~Items()
{
	
}
