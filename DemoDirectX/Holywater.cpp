#include "Holywater.h"

Holywater::Holywater()
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(22));
	isDone = true;
	isFire = false;
}
void Holywater::collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> COOBJECTS;
	COOBJECTS.clear();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) != dynamic_cast<Gate*>(coObjects->at(i)) /*&& coObjects->at(i) != dynamic_cast<Ground*>(coObjects->at(i))*/)
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
					//isDone = true;
					//isFire = false;
				}
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (state == 0 /*&& e->ny == -1*/)
				{
					/*isDone = true;
					isFire = false;*/
					this->SetState(1);
				}
			}
			else if (dynamic_cast<Knight*>(e->obj))
			{
				Knight *knight = dynamic_cast<Knight*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					knight->loseHp(2);
					if (knight->GetState() != knight_ani_die)
						listHit.push_back(CreateHit(knight->GetPositionX(), knight->GetPositionY() + 10));


					if (knight->getHp() <= 0)
						knight->SetState(knight_ani_die);
				}
			}
			else if (dynamic_cast<Bat*>(e->obj))
			{
				Bat  *bat = dynamic_cast<Bat*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					bat->loseHp(1);
					if (bat->GetState() != bat_ani_die)
						listHit.push_back(CreateHit(bat->GetPositionX(), bat->GetPositionY() + 10));


					if (bat->getHp() <= 0)
						bat->SetState(bat_ani_die);
					
				}
			}
			else if (dynamic_cast<Monkey*>(e->obj))
			{
				Monkey *monkey = dynamic_cast<Monkey*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					monkey->loseHp(1);
					if (monkey->GetState() != monkey_ani_die)
						listHit.push_back(CreateHit(monkey->GetPositionX(), monkey->GetPositionY() + 10));


					if (monkey->getHp() <= 0)
						monkey->SetState(monkey_ani_die);
					
				}
			}
			else if (dynamic_cast<Frog*>(e->obj))
			{
				Frog *frog = dynamic_cast<Frog*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					frog->loseHp(2);
					if (frog->GetState() != frog_ani_die)
						listHit.push_back(CreateHit(frog->GetPositionX(), frog->GetPositionY() + 10));


					if (frog->getHp() <= 0)
						frog->SetState(frog_ani_die);
					
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}
void Holywater::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (isHolyWaterShattered == true && GetTickCount() - holyWaterShatteredCounter > 1500)
	{
		this->SetState(0);
		isHolyWaterShattered = false;
		holyWaterShatteredCounter = 0;
		this->isDone = true;
		this->isFire = false;
		return;
	}

	if (CheckPosKnife(POSX) == true)
	{
		isDone = true;
		isFire = false;
		return;
	}

	SubWeapon::Update(dt);

	vy += 0.001 * dt;

	collisionwith(dt, coObjects);

}

void Holywater::Render()
{
	if (!isDone && !CheckPosKnife(POSX))
		SubWeapon::Render();

	SubWeapon::renderlisthit();

	//RenderBoundingBox();
}

void Holywater::SetV()
{
	if (nx > 0)
	{
		vx = 0.3;
		vy = -0.1;
	}
	else
	{
		vx = -0.25;
		vy = -0.1;
	}

}

void Holywater::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isDone)
	{
		left = x;
		top = y;
		right = left + 32;
		bottom = top + 28;
	}

}
void Holywater::SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding)
{
	SubWeapon::SetPosSubWeapon(D3DXVECTOR3(pos.x, pos.y, 0), isstanding);
	POSX = pos.x;
}
void Holywater::SetState(int State)
{
	CGameObject::SetState(State);

	switch (State)
	{
	case 0:
		break;
	case 1:
		StartHolyWater();
		break;
	default:
		break;
	}
}

bool Holywater::CheckPosKnife(float a)
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



Holywater::~Holywater()
{
}
