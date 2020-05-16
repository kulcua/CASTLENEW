#include "Knight.h"



Knight::Knight(float maxX1, float maxX2)
{
	this->maxX1 = maxX1;
	this->maxX2 = maxX2;
	hp = 3;
	damage = 2;
	SetState(knight_ani_run);
	score = 400;
}

void Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	
	if (state == knight_ani_die && animation_set->at(knight_ani_die)->RenderOver(knight_ani_die_time))
	{
		isDone = true;
		return;
	}
	Enemy::Update(dt);

	if(state!= knight_ani_die)
		vy += 0.00015 * dt;
	
	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	vector<LPGAMEOBJECT> COOBJECTS;
	COOBJECTS.clear();

	for (int i = 0; i < coObject->size(); i++)
	{
		if (coObject->at(i) != dynamic_cast<Bat*>(coObject->at(i)))
		{
			COOBJECTS.push_back(coObject->at(i));
		}
	}

	if(state!=1)
		CalcPotentialCollisions(&COOBJECTS, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);


		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx != 0 && ny == 0)
		{
			this->nx *= -1;
			this->vx *= -1;
		}
		else*/ if (ny == -1.0f)
		{
			vy = 0;
		}
	}

	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];


	if (x <= maxX1 || x >= maxX2)
	{
		this->nx *= -1;
		this->vx *= -1;
	}
}

void Knight::Render()
{
	if(!isDone)
		animation_set->at(state)->Render(nx, x, y);
	else return;
	//RenderBoundingBox();
}

void Knight::SetState(int State)
{
	Enemy::SetState(State);
	switch (State)
	{
	case knight_ani_run:
		if (nx > 0) vx = knight_speed;
		else vx = -knight_speed;
		vy = 0;
		break;
	case knight_ani_die:
		vx = vy = 0;
		animation_set->at(State)->StartAni();
		break;
	default:
		break;
	}
}

int Knight::getHp()
{
	return Enemy::getHp();
}

void Knight::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state != knight_ani_die)
	{
		l = x;
		t = y;
		r = l + knight_box_width;
		b = t + knight_box_height;
	}
}

void Knight::loseHp(int x)
{
	Enemy::loseHp(x);
}
Knight::~Knight()
{
}
