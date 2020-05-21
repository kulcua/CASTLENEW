#include "Boomerang.h"

Boomerang::Boomerang(LPGAMEOBJECT simon)
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(21));
	isDone = true;
	isFire = false;
	this->simon = simon;
}

void Boomerang::collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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
	COOBJECTS.push_back(simon);

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

				}
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				this->isDone = true;
				this->isFire = false;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	SubWeapon::Update(dt);

	if (nx > 0) vx -= 0.01;
	else vx += 0.01;

	if (CheckPosKnife(POSX))
	{
		isFire = false;
		isDone = true;
		return;
	}
	collisionwith(dt, coObjects);
}

void Boomerang::Render()
{
	if (!isDone && !CheckPosKnife(POSX))
		SubWeapon::Render();

	SubWeapon::renderlisthit();

	RenderBoundingBox();
}

void Boomerang::SetV()
{
	if (nx > 0) vx = 0.5;
	else vx = -0.5;
	vy = 0;
}

void Boomerang::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isDone)
	{
		left = x;
		top = y;
		right = left + 28;
		bottom = top + 28;
	}
}
void Boomerang::SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding)
{
	SubWeapon::SetPosSubWeapon(D3DXVECTOR3(pos.x, pos.y, 0), isstanding);
	POSX = pos.x;
}

bool Boomerang::CheckPosKnife(float a)
{
	if (vx > 0)
	{
		if (x - a >= ((530 / 2)))
			return true;
	}
	else if (vx < 0)
	{
		if (a - x >= ((530 / 2)))
			return true;
	}
	return false;
}


Boomerang::~Boomerang()
{
}
