#include "Bone.h"



Bone::Bone()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(27));
	//isDone = true;
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
	//vy += 0.9*dt;
	vy += 0.001*dt;
	x += dx;
	y += dy;

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
	case 0:
		if (nx > 0)
			vx = 0.15;
		else
			vx = -0.15;
		vy = -0.45;
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
		r = l + 32;
		b = t + 32;
	}
}

void Bone::setpos(D3DXVECTOR2 pos)
{
	SetPosition(pos.x+5, pos.y+5);
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
