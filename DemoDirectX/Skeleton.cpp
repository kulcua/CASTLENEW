#include "Skeleton.h"



Skeleton::Skeleton(LPGAMEOBJECT simon)
{
	this->simon = simon;
	score = 300;
	hp = 1;
	damage = 3;
	bone = new Bone();
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
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


	if (bone->isDone)
		bone->isDone = false;

	if (!CheckCam() && state == 0)
		state = skeleton_ani_die;

	if (state == skeleton_ani_die && animation_set->at(skeleton_ani_die)->RenderOver(skeleton_time))
	{
		isDone = true;
		return;
	}
	
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

		x += min_tx * dx + nx * 1.0f;
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

		}
	
	}


	Enemy::Update(dt);
	if(state!=skeleton_ani_die)
		vy += 0.001 * dt;

	

	//if (simon->GetPositionX() < 200 && !check)
		//check = true;

	if (x >= simon->GetPositionX())
		nx = -1;
	else
		nx = 1;

	if (abs(x - simon->GetPositionX()) < 270)
	{
		if (abs(simon->GetPositionX() - x) > 180)
		{
			if (coEvents.size() != 0 && state != skeleton_ani_die)
				vx = 0.13 * nx;
		}

		if (abs(simon->GetPositionX() - x) < 60)
		{
			if (coEvents.size() != 0 && state != skeleton_ani_die)
				vx = -0.15 * nx;
		}

		if (jump&&y > 250 && state != skeleton_ani_die)
		{
			vy = -0.4;
			nhay1lan = jump = false;
		}
	}

}

void Skeleton::Render()
{
	bone->Render();
	if (!isDone)
	{	
		animation_set->at(state)->Render(nx, x, y);
	}
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
	case skeleton_ani_die:
		vx = vy = 0;
		animation_set->at(State)->StartAni();
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
	if (state != skeleton_ani_die)
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
	return (y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT)-10);
}

void Skeleton::loseHp(int x)
{
	Enemy::loseHp(x);
}

Skeleton::~Skeleton()
{
}
