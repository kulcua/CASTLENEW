#include "Monkey.h"



Monkey::Monkey(LPGAMEOBJECT simon)
{
	this->simon = simon;
	hp = 1;
	score = 500;
	damage = 3;
}

void Monkey::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{

	if (state == 1 && animation_set->at(1)->RenderOver(300))
	{
		isDone = true;
		return;
	}

	Enemy::Update(dt);
	if(state!=1)
		vy += 0.002 * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT> COOBJECTS;
	COOBJECTS.clear();

	for (int i = 0; i < coObject->size(); i++)
	{
		if (coObject->at(i) != dynamic_cast<Frog*>(coObject->at(i)))
		{
			COOBJECTS.push_back(coObject->at(i));
		}
	}

	

	CalcPotentialCollisions(&COOBJECTS, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		/*vy += 0.002 * dt;*/
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		
		//if (ny!=0)
		{
			if (ny == -1)
			{
				vy = 0;
				//jump = true;
			}
		}
		
	}

	
	//else
	if(CheckCam() && state != monkey_ani_die)
	{
		if (x < simon->GetPositionX())
		{
			if (abs(simon->GetPositionX() - x) > 100)
			{
				vx = 0.16;
				nx = 1;
			}
		}
		else if (x > simon->GetPositionX())
		{
			if ((abs(simon->GetPositionX() - x) > 100))
			{
				vx = -0.16;
				nx = -1;
			}
		}
		
		if (((rand() % 10000 < 350)&& (abs(simon->GetPositionX() - x) < 120)) && y > 350/*330*/)
		{
			vy = -0.35;
			//jump = false;
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];


}

void Monkey::Render()
{
	if (!isDone)
		animation_set->at(state)->Render(nx, x, y);
	else return;
	//RenderBoundingBox();
}

void Monkey::SetState(int State)
{
	Enemy::SetState(State);
	switch (State)
	{
	case 0:
		vx = vy = 0;
		break;
	case 1:
		vx = vy = 0;
		animation_set->at(State)->StartAni();
		break;
	default:
		break;
	}
}

int Monkey::getHp()
{
	return Enemy::getHp();
}

void Monkey::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state !=1 )
	{
		l = x;
		t = y;
		r = l + 32;
		b = t + 32;
	}
}

bool Monkey::CheckCam()
{
	CGame *game = CGame::GetInstance();
	return (x >= game->GetCamPosX()+50 && x < game->GetCamPosX() + (SCREEN_WIDTH) && y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT));
}

void Monkey::loseHp(int x)
{
	Enemy::loseHp(x);
}


Monkey::~Monkey()
{
}
