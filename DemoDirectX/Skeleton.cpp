#include "Skeleton.h"



Skeleton::Skeleton(LPGAMEOBJECT simon)
{
	this->simon = simon;
	bone = new Bone();
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	Enemy::Update(dt);
	vy += 0.002 * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

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

		x += min_tx * dx + nx * 0.6f;
		y += min_ty * dy + ny * 0.6f;

		if (ny!=0)
		{
			if (ny == -1)
				vy = 0;
			else
				jump = true;
		}

	}
	

	if (simon->GetPositionX() < 200 && !check)
		check = true;



	if (CheckCam()&&x<1300)
	{
		if (simon->GetPositionX() < x)
		{
			if (abs(simon->GetPositionX() - x) >= 15 && abs(simon->GetPositionX() - x) <= 65)
			{
				nx = -1;
				vx = 0.25;
			}
			else if (abs(simon->GetPositionX() - x) >= 2 && abs(simon->GetPositionX() - x) < 15)
			{
				nx = 1;
				vx = -0.25;
			}

		}
		else if (simon->GetPositionX() > x)
		{
			/*nx = 1;
			vx = 0.2;*/
		}
		/*else
			vx = -0.1;*/

		

		//	/*else if (abs(simon->GetPositionX() - x) > 100)
		//	{
		//		nx = -1;
		//		vx = -0.4;
		//	}*/
		//	/*if ((abs(simon->GetPositionX() - x) > 120))
		//	{
		//		vx = -0.2;
		//		nx = -1;
		//	}*/
		//}

		if (/*(rand() % 10000 < 150)*/ jump/*&& y > 300*/)
		{
			vy = -0.4;
			jump = false;
		}
	}
	else 
	{ /*vx = -0.08;*/
	
		if (simon->GetPositionX() > x&&check)
			vx = 0.12;
		else if (check&&simon->GetPositionX() < x)
			vx = -0.08;
	}

	

	if (!bone->isDone)
	{	
		
		if (!bone->getcheck())
		{
			bone->SetNx(nx);
			bone->setpos(D3DXVECTOR2(x, y));
			bone->SetState(0);
			bone->setcheck(true);
		}
		
		bone->Update(dt, coObject);
	}

	
	if(bone->isDone)
		bone->isDone = false;
}

void Skeleton::Render()
{
	bone->Render();
	if (!isDone)
		animation_set->at(state)->Render(nx, x, y);
	else return;
	//RenderBoundingBox();
}

void Skeleton::SetState(int State)
{
	Enemy::SetState(State);
	switch (State)
	{
	case 0:
		vx = vy = 0;
		break;
	default:
		break;
	}
}

int Skeleton::getHp()
{
	return Enemy::getHp();
}

void Skeleton::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	//if (state != 1)
	{
		l = x;
		t = y;
		r = l + 32;
		b = t + 62;
	}
}

bool Skeleton::CheckCam()
{
	CGame *game = CGame::GetInstance();
	return (x >= game->GetCamPosX() + 20 && x < game->GetCamPosX() + (SCREEN_WIDTH)-50 && y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT));
}

void Skeleton::loseHp(int x)
{
	Enemy::loseHp(x);
}

Skeleton::~Skeleton()
{
}
