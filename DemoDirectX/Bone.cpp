#include "Bone.h"
#include"Simon.h"


Bone::Bone(LPGAMEOBJECT simon)
{
	this->s = simon;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(bone_aniset));
	//isDone = true;
	damage = bone_hp;
	hp = 1;
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	
	
	if (!CheckCam())
	//if(checkposX(POSX))
	{
		checkset = false;
		this->isDone = true;
		return;
	}

	Enemy::Update(dt);
	vy += bone_gravity * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	vector<LPGAMEOBJECT> COOBJECTS;
	COOBJECTS.clear();

	COOBJECTS.push_back(s);

	CalcPotentialCollisions(&COOBJECTS, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Simon*>(e->obj))
			{
				Simon* simon = dynamic_cast<Simon*>(e->obj);
				if (simon->untouchtime->IsTimeUp() && simon->GetState() != simon_ani_led && simon->watertime->IsTimeUp())
				{
					simon->untouchtime->Start();
					simon->loseHp(this->damage);
					if (!simon->isStandOnStair || simon->GetHealth() == 0)
					{
						if (nx != 0)
						{
							if (nx == 1)
								simon->SetNx(-1);
							else
								simon->SetNx(1);
						}
						simon->SetState(simon_ani_hurt);
					}
				}
				else
				{
					if (nx != 0)
						x += dx;
					if (ny != 0)
						y += dy;
				}
			}
		}
	}
	
}

void Bone::Render()
{
	if (!isDone&&!checkposX(POSX))
		animation_set->at(state)->Render(nx, x, y);
	else return;
	//RenderBoundingBox();
}

void Bone::SetState(int State)
{
	Enemy::SetState(State);
	switch (State)
	{
	case bone_ani_fly:
		if (nx > 0)
			vx = bone_vx;
		else
			vx = -bone_vx;
		vy = bone_vy;
		break;
	default:
		break;
	}
}



void Bone::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isDone)
	{
		l = x;
		t = y;
		r = l + bone_box_width;
		b = t + bone_box_height;
	}
}

void Bone::setpos(D3DXVECTOR2 pos)
{
	SetPosition(pos.x+ add_dis, pos.y+ add_dis);
	POSX = pos.x;
}
bool Bone::CheckCam()
{
	CGame *game = CGame::GetInstance();
	return (x >= game->GetCamPosX()  && x < game->GetCamPosX() + (SCREEN_WIDTH) && y >= game->GetCamPosY() && y < game->GetCamPosY() + (SCREEN_HEIGHT));
}
Bone::~Bone()
{
}
