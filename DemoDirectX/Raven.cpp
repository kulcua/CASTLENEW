#include "Raven.h"



Raven::Raven(LPGAMEOBJECT simon)
{
	this->simon = simon;
	hp = 1;
	damage = 3;
	score = 200;
}


void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	Enemy::Update(dt);

	if (!CheckCam()&&state==raven_ani_fly)
		state = raven_ani_die;

	if (state == raven_ani_die && animation_set->at(raven_ani_die)->RenderOver(raven_time))
	{
		isDone = true;
		return;
	}


	if (CheckCam()&&state!=raven_ani_die)
		state = raven_ani_fly;

	if(CheckCam()&&state!=raven_ani_die)
	{
		if(!timeflydown)
			timerstopdown->Start();
		int a = 30 + rand() % 400;
		int b = 40 + rand() % 400;
		if (x < simon->GetPositionX())
		{
			//float a = 30 + rand() % 120;//% 91;
			if (abs(simon->GetPositionX() - x) > a/*100*/)
			{
				wait->Start();
				if (!wait1->IsTimeUp())
					vx = 0;
				else
					vx = 0.2;
				nx = 1;
			}
		}
		else if (x > simon->GetPositionX())
		{
			if (abs(simon->GetPositionX() - x) > b)
			{
				wait1->Start();
				if (!wait->IsTimeUp())
				{
					vx = 0;		
				}
				else {
					vx = -0.2;
					
				}
					
				nx = -1;
			}
		}

		int c = 40 + rand() % 69;
		int d = 30 + rand() % 31;
		if (checkstar)
		{
			if (simon->GetPositionY() - c/*20*/ > y)
			{
				dow->Start();
				if (!dow1->IsTimeUp())
					vy = 0;
				else
					vy = 0.1;
			}
			else if (simon->GetPositionY() + d/*50*/ < y)
			{
				dow1->Start();
				if (!dow->IsTimeUp())
					vy = 0;
				else
					vy = -0.1;
			}
		}

		


		if (timerstopdown->IsTimeUp())
			checkstar = true;

		timeflydown = true;

	}

	if (state != raven_ani_die)
	{
		x += dx;
		y += dy;
	}
}

void Raven::Render()
{
	if (!isDone)
		animation_set->at(state)->Render(nx, x, y);
	else return;
	//RenderBoundingBox();
}

void Raven::SetState(int State)
{
	Enemy::SetState(State);
	switch (State)
	{
	case 0:
		vx = vy = 0;
		break;
	case 1:
		break;
	case 2:
		vx = vy = 0;
		animation_set->at(State)->StartAni();
		break;
	}
}

bool Raven::CheckCam()
{
	CGame *game = CGame::GetInstance();
	return (x >= game->GetCamPosX() + 10 && x < game->GetCamPosX() + (SCREEN_WIDTH) && y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT - 10));
}

int Raven::getHp()
{
	return Enemy::getHp();
}

void Raven::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state != raven_ani_die)
	{
		l = x;
		t = y;
		r = l + 32;
		b = t + 32;
	}
}

void Raven::loseHp(int x)
{
	Enemy::loseHp(x);
}


Raven::~Raven()
{
}
