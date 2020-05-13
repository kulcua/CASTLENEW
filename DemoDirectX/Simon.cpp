#include "Simon.h"
#include"Ground.h"


Simon::Simon() 
{
	whip = new Whip();
	knife = new Knife();
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(simon_ani_set));
	currentWeapon = -1;
	isGrounded = false;
	health = simon_max_health;
	//state = simon_ani_idle;
	score = 0;
	mana = 5;
	life = 3;
	this->nextscene = 1;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	

	if (x < max_screen_left) //để cài không cho simon đi ngược màn hình
		x = max_screen_left;

	


	if (isStandOnStair == false && isWalkStair == false)
		vy += simon_gravity * dt;

	
	if (isWalkStair == true)
		DoAutoWalkStair();

	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	vector<LPGAMEOBJECT> COOBJECTS;
	COOBJECTS.clear();


	for (int i = 0; i < coObjects->size(); i++)
	{	
		if (coObjects->at(i)!= dynamic_cast<Candle*>(coObjects->at(i)))
		{
			COOBJECTS.push_back(coObjects->at(i));
		}
	}

	
	CalcPotentialCollisions(&COOBJECTS/*coObjects*/, coEvents);


	if (coEvents.size() == 0 && !isWalkStair)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		
		if (!isWalkStair)
		{
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
		}

		/*if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground *>(e->obj))
			{
				//Ground *ground = dynamic_cast<Ground *>(e->obj);

				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						isGrounded = true;
						vy = 0;
					}
					else
						y += dy;
				}

				if (state == simon_ani_stair_up || state == simon_ani_stair_down)
				{
					if (nx != 0) x -= nx * 0.4f;
				}
				
			}
			else if (dynamic_cast<Gate*>(e->obj))
			{

				Gate *p = dynamic_cast<Gate*>(e->obj);
				this->nextscene = p->nextscene;
				isChangeScene = true;
				isWalkStair = false;
				
			}
		}
	}
	

	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];
}

void Simon::Render()
{
	animation_set->at(state)->Render(nx, x, y);
	
	//RenderBoundingBox();
}


void Simon::SetState(int State)
{
	CGameObject::SetState(State);
	switch (State)
	{
	case simon_ani_run:
		isStandOnStair = false;
		if (nx > 0)
			vx = simon_run;
		else
			vx = -simon_run;
		break;
	case simon_ani_idle:
		this->StairLeftFirst = false;
		vx = 0;
		isStandOnStair = false;
		break;
	case simon_ani_jump:
		if (isGrounded)
		{
			isGrounded = false;
			vy = simon_jump;
		}
		break;
	case simon_ani_sit:
		isStandOnStair = false;
		vx = 0;
		break;
	case simon_ani_stand_hit:
		isStandOnStair = false;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_sit_hit:	
		vx = 0;
		isStandOnStair = false;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_led:
		vx = 0;
		isStandOnStair = false;
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_stair_up:
		isStandOnStair = true;
		if (nx > 0) vx = simon_stair;
		else vx = -simon_stair;
		vy = -simon_stair;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_stair_down:
		isStandOnStair = true;
		if (nx > 0) vx = simon_stair;
		else vx = -simon_stair;
		vy = simon_stair;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_stair_down_hit:
		vx = vy = 0;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_stair_up_hit:
		vx = vy = 0;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	}

}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x+15;
	top = y;
	right = left +simon_box_width;
	bottom = top +simon_box_height;
}

void Simon::SimonColliWithItems(vector<LPGAMEOBJECT> *listitems)//hàm này để tránh việc ko xét va chạm dc khi 2 simon trùng với items
{
	float l_items, t_items, r_items, b_items, l_simon, t_simon, r_simon, b_simon;
	GetBoundingBox(l_simon, t_simon, r_simon, b_simon);

	for (UINT i = 0; i < listitems->size(); i++)
	{
		LPGAMEOBJECT e = listitems->at(i);
		e->GetBoundingBox(l_items, t_items, r_items, b_items);
		if (CGameObject::AABBCheck(l_simon, t_simon, r_simon, b_simon, l_items, t_items, r_items, b_items))
		{
			if (e->GetState() == items_for_whip)
			{
				SetState(simon_ani_led);
				e->isDone = true;
				if (whip->GetState() == whip_lv1)
					whip->SetState(whip_lv2);
				else if (whip->GetState() == whip_lv2)
					whip->SetState(whip_lv3);
				

			}
			else if (e->GetState() == items_big_heart)
			{
				
				e->isDone = true;
				//health = health - 1;
				mana += 5;
			}
			else if (e->GetState() == items_knife)
			{
				e->isDone = true;
				currentWeapon = weapon_knfie;
				//e->SetState(knife_ani);				
			}
			else if (e->GetState() == items_watch)
			{
				e->isDone = true;
				currentWeapon = weapon_watch;
			}
		}
		



	}
}

bool Simon::SimonColliWithStair(vector<LPGAMEOBJECT> *liststair)
{
	canmovedownstair = canmoveupstair = false;
	float l_simon, t_simon, r_simon, b_simon;
	GetBoundingBox(l_simon, t_simon, r_simon, b_simon);
	t_simon += 50; // để không va chạm với cục thang phía trên
	b_simon += 10;
	r_simon += 15; //mở rộng vùng va chạm thêm 
	for (UINT i = 0; i < liststair->size(); i++)
	{
		float l_stair, t_stair, r_stair, b_stair;
		liststair->at(i)->GetBoundingBox(l_stair, t_stair, r_stair, b_stair);

		if (CGameObject::AABBCheck(l_simon, t_simon, r_simon, b_simon, l_stair, t_stair, r_stair, b_stair))
		{
			
			stairNx = liststair->at(i)->stairdir;

			stairCollided = liststair->at(i);

			if ((b_simon) < b_stair)  canmovedownstair = true; 
			

			float upstair_x = -999999, upstair_y = -999999; // toạ độ của bậc thang liền kề

			for (UINT j = 0; j < liststair->size(); j++)
			{
				if (i == j)
					continue;

				liststair->at(j)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - l_stair);
				float dy = upstair_y - t_stair;

				if (dx == stair_box_width && dy == -stair_box_height)
					canmoveupstair = true;
				if (dx == stair_box_width && dy == stair_box_height)
					canmovedownstair = true;
			}

			return true; 
		}
	}

	return false;
}

void Simon::AutoWalkStair(float newPosX, int stateAfterAutoWalk, int nxAfterAutoWalk)
{
	this->isWalkStair = true;
	this->newPosX = newPosX;
	this->stateAfterAutoWalk = stateAfterAutoWalk;
	this->nxAfterAutoWalk = nxAfterAutoWalk;
}

void Simon::DoAutoWalkStair()
{
	if (x != newPosX)
	{
		x += dx;
		y += dy;
		if ((nx == 1 && x >= newPosX) || (nx == -1 && x <= newPosX))
		{
			x = newPosX;
			state = stateAfterAutoWalk;
			nx = nxAfterAutoWalk;
			SetState(state);
			if (state == simon_ani_stair_down) 
				y += 1.0f; // để đảm bảo simon sẽ va chạm với bậc thang 
			isWalkStair = false;

		}
	}
}

void Simon::StandOnStair()
{
	vx = vy = 0;
}

Simon::~Simon()
{

}
