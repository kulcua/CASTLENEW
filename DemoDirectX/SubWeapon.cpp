#include "SubWeapon.h"

SubWeapon::SubWeapon()
{

}

Hit* SubWeapon::CreateHit(float x, float y)
{
	return new Hit(x, y);
}



void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects)
{
	CGameObject::Update(dt);
}

void SubWeapon::SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding)
{
	if (!isstanding)
		pos.y += 25;
	else
		pos.y += 10;
	SetPosition(pos.x, pos.y);
}

void SubWeapon::renderlisthit()
{
	for (int i = 0; i < listHit.size(); i++)
		listHit[i]->Render();
}

void SubWeapon::Render()
{

	animation_set->at(state)->Render(nx, x, y);
	//RenderBoundingBox();
}



void SubWeapon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
}

void SubWeapon::collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> COOBJECTS;
	COOBJECTS.clear();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) != dynamic_cast<Gate*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Ground*>(coObjects->at(i)))
		{
			COOBJECTS.push_back(coObjects->at(i));
		}
	}


	CalcPotentialCollisions(&COOBJECTS, coEvents);



	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Candle *>(e->obj))
			{
				Candle *candle = dynamic_cast<Candle *>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					listHit.push_back(CreateHit(candle->GetPositionX(), candle->GetPositionY() + 10));
					candle->SetState(break_candle);
					isDone = true;
					isFire = false;
				}

			}

		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}


SubWeapon::~SubWeapon()
{
}
