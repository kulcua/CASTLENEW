#include "Skeleton.h"



Skeleton::Skeleton(LPGAMEOBJECT simon)
{
	this->simon = simon;
	bone = new Bone();
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT> COOBJECTS;
	coEvents.clear();




	COOBJECTS.clear();

	for (int i = 0; i < coObject->size(); i++)
	{
		if (coObject->at(i) == dynamic_cast<Ground*>(coObject->at(i)))
			COOBJECTS.push_back(coObject ->at(i));
	}

	CalcPotentialCollisions(&COOBJECTS/*coObject*/, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		
		if(nhay1lan) //cham đất thì mới nhảy
			jump = true;
	}
	else
	{
		
		float min_tx, min_ty, nx = 0, ny=0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;
		



		

		if (ny != 0)
		{

			if (ny == -1)
			{
				vy = 0;
				nhay1lan = true;
			}
			else
				y += dy;

			/*if (ny == 1)
			{
				vy = 0.0015*dt;
			}*/
		}
		

		/*if ((ny==0))
			jump = true;*/
		//DebugOut(L"NY SKELETON %f\n",ny);
		//DebugOut(L"NX  SKELETON %f\n", nx);
	}

	Enemy::Update(dt);
	vy += 0.001 * dt;

	

	if (simon->GetPositionX() < 200 && !check)
		check = true;



	if (CheckCam()&&x<1300)
	{
		if (simon->GetPositionX() < x)
		{
			if (abs(simon->GetPositionX() - x) >= 15 && abs(simon->GetPositionX() - x) <= 65)
			{
				nx = -1;
				vx = 0.1;
			}
			else if (abs(simon->GetPositionX() - x) >= 2 && abs(simon->GetPositionX() - x) < 15)
			{
				nx = 1;
				vx = -0.1;
			}
			
		}
		

		


		if (/*(rand() % 10000 < 150)*/ jump/*&& y > 300*/)
		{
			vy = -0.4;
			nhay1lan=jump = false;
		}

	}
	else 
	{ 

		if (simon->GetPositionX() > x&&check)
			vx = 0.1;
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
	RenderBoundingBox();
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
