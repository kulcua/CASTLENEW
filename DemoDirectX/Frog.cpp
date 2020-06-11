#include "Frog.h"



Frog::Frog(LPGAMEOBJECT simon)
{
	this->simon = simon;
	hp = 3;
	damage = 3;
	score = 300;
}
void Frog::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject,bool clk)
{

	Enemy::Update(dt);

	if (state == frog_ani_die && animation_set->at(frog_ani_die)->RenderOver(300))
	{
		isDone = true;
		return;
	}

	if (clk)
		return;

	if (x - simon->GetPositionX() > 200&&!check)
		check = true;
	

	/*if(CheckCam())*/if (CheckCam()&&check&&state!=frog_ani_die)
	{
		if (GetTickCount() - timestop >= 150 && state != frog_ani_die)
		{
			if (nx > 0) vx = 0.08;
			else vx = -0.08;


			if (simon->GetPositionY() - 20 > y)
				vy = 0.05;
			else if (simon->GetPositionY() + 20 < y)
				vy = -0.05;

		}




		if (x < simon->GetPositionX())
		{
			if (abs(simon->GetPositionX() - x) > 40)
			{
				//vx = 0.08;
				nx = 1;
			}
		}
		else if (x > simon->GetPositionX())
		{
			if ((abs(simon->GetPositionX() - x) > 40))
			{
				//vx = -0.08;
				nx = -1;
			}
		}
		

		/*if (simon->GetPositionY()-20>y)
				vy = 0.05;
		else if (simon->GetPositionY()+20 < y)
				vy = -0.05;*/


	}


	x += dx;
	y += dy;

}

void Frog::Render()
{
	if (!isDone&&check)
		animation_set->at(state)->Render(nx, x, y);
	else return;
	//RenderBoundingBox();
}

void Frog::SetState(int State)
{
	Enemy::SetState(State);
	switch (State)
	{
	
	case 1:
		vx = vy = 0;
		animation_set->at(State)->StartAni();
		break;
	}
}

bool Frog::CheckCam()
{
	CGame *game = CGame::GetInstance();
	return (x >= game->GetCamPosX() && x < game->GetCamPosX() + (SCREEN_WIDTH) && y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT));
}

int Frog::getHp()
{
	return Enemy::getHp();
}

void Frog::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (check&&state!=frog_ani_die)
	{
		l = x;
		t = y;
		r = l + 32;
		b = t + 32;
	}
}

void Frog::loseHp(int x)
{
	Enemy::loseHp(x);
}

Frog::~Frog()
{
}
