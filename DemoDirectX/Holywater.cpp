#include "Holywater.h"

Holywater::Holywater()
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(holy_aniset));
	isDone = true;
	isFire = false;
	holyWaterShatteredCounter = 0;
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
		if (coObjects->at(i) != dynamic_cast<Gate*>(coObjects->at(i))/*&& coObjects->at(i) != dynamic_cast<Ground*>(coObjects->at(i))*/)
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
			else if (dynamic_cast<Ground*>(e->obj)||dynamic_cast<BreakWall*>(e->obj)||dynamic_cast<GroundMoving*>(e->obj))
			{
				if (state == 0 /*&& e->ny == -1*/)
				{
					/*isDone = true;
					isFire = false;*/
					this->SetState(holy_ani_break);
				}
			}
			else if (dynamic_cast<Knight*>(e->obj))
			{
				Knight *knight = dynamic_cast<Knight*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					knight->loseHp(dame_into_knight);
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
					bat->loseHp(dame_into_bat);
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
					monkey->loseHp(dame_into_monkey);
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
					frog->loseHp(dame_into_frog);
					if (frog->GetState() != frog_ani_die)
						listHit.push_back(CreateHit(frog->GetPositionX(), frog->GetPositionY() + 10));


					if (frog->getHp() <= 0)
						frog->SetState(frog_ani_die);
					
				}
			}
			else if (dynamic_cast<SmallCandle *>(e->obj))
			{
				SmallCandle *candle = dynamic_cast<SmallCandle *>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					listHit.push_back(CreateHit(candle->GetPositionX(), candle->GetPositionY() + 10));
					candle->SetState(break_candle);
				}

			}
			else if (dynamic_cast<Skeleton*>(e->obj))
			{
				Skeleton *skele = dynamic_cast<Skeleton*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					skele->loseHp(dame_into_skele);
					if (skele->GetState() != skeleton_ani_die)
						listHit.push_back(CreateHit(skele->GetPositionX(), skele->GetPositionY() + 10));


					if (skele->getHp() <= 0)
						skele->SetState(skeleton_ani_die);
				}
			}
			else if (dynamic_cast<Raven*>(e->obj))
			{
				Raven *raven = dynamic_cast<Raven*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					raven->loseHp(dame_into_raven);
					if (raven->GetState() != raven_ani_die)
						listHit.push_back(CreateHit(raven->GetPositionX(), raven->GetPositionY() + 10));


					if (raven->getHp() <= 0)
						raven->SetState(raven_ani_die);
				}
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				Zombie *zombie = dynamic_cast<Zombie*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					zombie->colliwhip = true;
					zombie->loseHp(dame_into_zombie);
					if (zombie->GetState() != zombie_ani_die)
						listHit.push_back(CreateHit(zombie->GetPositionX(), zombie->GetPositionY() + 10));

					if (zombie->getHp() <= 0)
						zombie->SetState(zombie_ani_die);
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}
void Holywater::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	SubWeapon::ClearListHit();
	if (isHolyWaterShattered == true && GetTickCount() - holyWaterShatteredCounter > 1500)
	{
		this->isDone = true;
		this->isFire = false;
		this->SetState(holy_ani_not_break);
		isHolyWaterShattered = false;
		holyWaterShatteredCounter = 0;	
		return;
	}

	if (CheckPosKnife(POSX) == true)
	{
		isDone = true;
		isFire = false;
		return;
	}

	SubWeapon::Update(dt);

	vy += holy_gra * dt;

	collisionwith(dt, coObjects);

}

void Holywater::Render()
{
	if (!isDone && !CheckPosKnife(POSX))
		SubWeapon::Render();

	SubWeapon::renderlisthit();

	RenderBoundingBox();
}

void Holywater::SetV()
{
	if (nx > 0)
	{
		vx = holy_vx_right;
		vy = holy_vy;
	}
	else
	{
		vx = holy_vx_left;
		vy = holy_vy;
	}

}

void Holywater::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isDone)
	{
		left = x;
		top = y;
		right = left + holy_box_width;
		bottom = top + holy_box_height;
	}

}
void Holywater::SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding)
{
	SubWeapon::SetPosSubWeapon(D3DXVECTOR3(pos.x, pos.y, 0), isstanding);
	POSX = pos.x;
}

void Holywater::StartHolyWater()
{
	isHolyWaterShattered = true; 
	holyWaterShatteredCounter = GetTickCount();
}

void Holywater::SetState(int State)
{
	CGameObject::SetState(State);

	switch (State)
	{
	case holy_ani_not_break:
		break;
	case holy_ani_break:
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
		if (x - a >= ((SCREEN_WIDTH / 2)))
			return true;
	}
	else if (vx < 0)
	{
		if (a - x >= ((SCREEN_WIDTH / 2)))
			return true;
	}
	return false;
}



Holywater::~Holywater()
{
}
