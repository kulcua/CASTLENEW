#include "Skeleton.h"



Skeleton::Skeleton(LPGAMEOBJECT simon)
{
	this->simon = simon;
	score = skeleton_score;
	hp = skeleton_hp;
	damage = skeleton_damage;
	bone = new Bone(simon);
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject,bool clk)
{
	if (!bone->isDone)
	{

		if (!bone->getcheck())
		{
			bone->SetNx(nx);
			bone->setpos(D3DXVECTOR2(x, y));
			bone->SetState(bone_ani_fly);
			bone->setcheck(true);
		}

		bone->Update(dt, coObject);
	}


	if (bone->isDone)
		bone->isDone = false;

	if (!CheckCam()&& state == 0&&active)
		state = skeleton_ani_die;

	if (state == skeleton_ani_die && animation_set->at(skeleton_ani_die)->RenderOver(skeleton_time))
	{
		isDone = true;
		return;
	}

	if (clk)
		return;
	
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

		x += min_tx * dx + nx * 1.9f;
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
		vy += skeleton_gra * dt;

	


	if (x >= simon->GetPositionX())
		nx = -1;
	else
		nx = 1;

	if (abs(x - simon->GetPositionX()) < active_max_dis&&CheckCam())
	{
		active = true;
		if (abs(simon->GetPositionX() - x) > small_dis_one)
		{
			if (coEvents.size() != 0 && state != skeleton_ani_die)
				vx = skeleton_vx * nx;
		}
		else if (abs(simon->GetPositionX() - x) < small_dis_two)
		{
			if (coEvents.size() != 0 && state != skeleton_ani_die)
				vx = -skeleton_vx * nx;
		}





		if (jump&&y > max_y_jump && state != skeleton_ani_die)
		{
			vy = skeleton_vy;
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
	case skeleton_ani_idle:
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
		r = l + skeleton_box_width;
		b = t + skeleton_box_height;
	}
}

bool Skeleton::CheckCam()
{
	CGame *game = CGame::GetInstance();
	return (x >= game->GetCamPosX() + 5 && x < game->GetCamPosX() + (SCREEN_WIDTH)-20 &&y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT)-20);
}

void Skeleton::loseHp(int x)
{
	Enemy::loseHp(x);
}

Skeleton::~Skeleton()
{
}
