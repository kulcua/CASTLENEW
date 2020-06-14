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

void SubWeapon::ClearListHit()
{
	//DebugOut(L" SO PHAN TU TRONG LISTHIT %d \n", listHit.size());
	for (int i = 0; i < listHit.size(); i++)
	{
		if (listHit[i]->timedestroy())
			listHit.erase(listHit.begin() + i);
	}
	
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
		if (coObjects->at(i) != dynamic_cast<Gate*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Ground*>(coObjects->at(i))&&(coObjects->at(i) != dynamic_cast<BreakWall*>(coObjects->at(i))))
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
					isDone = true;
					isFire = false;
				}
			}
			else if (dynamic_cast<Bat*>(e->obj))
			{
				Bat  *bat = dynamic_cast<Bat*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					bat->loseHp(1);
					if(bat->GetState()!=bat_ani_die)
						listHit.push_back(CreateHit(bat->GetPositionX(), bat->GetPositionY() + 10));


					if (bat->getHp() <= 0)
						bat->SetState(bat_ani_die);
					isDone = true;
					isFire = false;
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
					isDone = true;
					isFire = false;
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
					isDone = true;
					isFire = false;
				}
			}
			else if (dynamic_cast<SmallCandle *>(e->obj))
			{
				SmallCandle *candle = dynamic_cast<SmallCandle *>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					listHit.push_back(CreateHit(candle->GetPositionX(), candle->GetPositionY() + 10));
					candle->SetState(break_candle);
					isDone = true;
					isFire = false;
				}

			}
			else if (dynamic_cast<Skeleton*>(e->obj))
			{
				Skeleton *skele = dynamic_cast<Skeleton*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					skele->loseHp(1);
					if (skele->GetState() != skeleton_ani_die)
						listHit.push_back(CreateHit(skele->GetPositionX(), skele->GetPositionY() + 10));


					if (skele->getHp() <= 0)
						skele->SetState(skeleton_ani_die);
					isDone = true;
					isFire = false;
				}
			}
			else if (dynamic_cast<Raven*>(e->obj))
			{
				Raven *raven = dynamic_cast<Raven*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					raven->loseHp(1);
					if (raven->GetState() != raven_ani_die)
						listHit.push_back(CreateHit(raven->GetPositionX(), raven->GetPositionY() + 10));


					if (raven->getHp() <= 0)
						raven->SetState(raven_ani_die);
					isDone = true;
					isFire = false;
				}
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				Zombie *zombie = dynamic_cast<Zombie*>(e->obj);

				if (e->nx != 0 || e->ny != 0)
				{
					zombie->colliwhip = true;
					zombie->loseHp(1);
					if (zombie->GetState() != zombie_ani_die)
						listHit.push_back(CreateHit(zombie->GetPositionX(), zombie->GetPositionY() + 10));

					if (zombie->getHp() <= 0)
						zombie->SetState(zombie_ani_die);
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
