#include "Simon.h"
#include"Ground.h"


Simon::Simon() 
{
	whip = new Whip();
	for (int i = 0; i < 3; i++)
	{
		knife[i] = new Knife();
		
		axe[i] = new Axe();

		boom[i] = new Boomerang(this);

		holywater[i] = new Holywater();
	}
	clk = new Clock();
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(simon_ani_set));
	currentWeapon = -1;
	isGrounded = false;
	health = simon_max_health;
	//state = simon_ani_idle;
	score = 0;
	mana = 99;
	life = 3;
	this->nextscene = 99;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects,bool clk)
{
	CGameObject::Update(dt);
	
	


	if (untouchtime->IsTimeUp())
		untouchtime->Stop();
	if (watertime->IsTimeUp())
		watertime->Stop();
	

	if (x < max_screen_left) //để cài không cho simon đi ngược màn hình
		x = max_screen_left;
	if (x > 1460)
		x = 1460;

	


	if (isStandOnStair == false && isWalkStair == false)
		vy += simon_gravity * dt;

	
	if (health == 0)
	{
		SetState(simon_ani_dead);
		return;
	}


	if (isWalkStair == true)
		DoAutoWalkStair();

	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	vector<LPGAMEOBJECT> COOBJECTS;
	COOBJECTS.clear();


	for (int i = 0; i < coObjects->size(); i++)
	{	
		if (coObjects->at(i)!= dynamic_cast<Candle*>(coObjects->at(i))&&coObjects->at(i) != dynamic_cast<SmallCandle*>(coObjects->at(i)) && (coObjects->at(i) != dynamic_cast<Monkey*>(coObjects->at(i))) && (coObjects->at(i) != dynamic_cast<Frog*>(coObjects->at(i))) && (coObjects->at(i) != dynamic_cast<Knight*>(coObjects->at(i)))&& coObjects->at(i) != dynamic_cast<Zombie*>(coObjects->at(i)))
		{
			COOBJECTS.push_back(coObjects->at(i));
		}
	}
	
	

	SimonColliWithMob(coObjects);

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

			if (dynamic_cast<Ground *>(e->obj)||dynamic_cast<BreakWall*>(e->obj))
			{
				//DebugOut(L"size: %d\n", coEvents.size());
				//Ground *ground = dynamic_cast<Ground *>(e->obj);

				if (e->ny != 0)
				{
					if (e->ny == -1&&(state!=simon_ani_hurt||(state==simon_ani_hurt&&vy>0)))
					{
						checkgroundmove = false;
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
				//isGrounded = true;//thử
				
			}
			else if(dynamic_cast<GroundMoving*>(e->obj))
			{					
				if (e->ny != 0)
				{
					DebugOut(L"size: %d\n", e->ny);
					if (e->ny == -1)
					{
						vx = e->obj->vx;
						checkgroundmove = true;
						isGrounded = true;				
						vy = 0;
					}
					else
						y += dy;
				}
				/*else
					vx = 0;*/
			}
			//else if (dynamic_cast<Knight*>(e->obj))
			//{
			//	if (untouchtime->IsTimeUp() && state != simon_ani_led && watertime->IsTimeUp())
			//	{
			//		untouchtime->Start();
			//		Knight* knight = dynamic_cast<Knight*>(e->obj);
			//		loseHp(knight->getDamage());
	
			//		{
			//			if (isStandOnStair == false || health == 0)
			//			{
			//				if (e->nx != 0)
			//				{
			//					if (e->nx == 1)
			//						SetNx(-1);
			//					else
			//						SetNx(1);
			//				}

			//				/*if (e->ny != 0)
			//					y += dy;*/
			//				SetState(simon_ani_hurt);
			//			}
			//		}
			//	}
			//	else
			//	{
			//		if (e->ny != 0)
			//			y += dy;
			//		if (e->nx != 0)
			//			x += dx;
			//	}
			//}
			else if (dynamic_cast<Bat*>(e->obj))
			{
				if (untouchtime->IsTimeUp() && state != simon_ani_led && watertime->IsTimeUp())
				{
					untouchtime->Start();
					Bat* bat = dynamic_cast<Bat*>(e->obj);
					//batdie = true;
					bat->Setcollisimon(true);
					bat->SetState(bat_ani_die);
					loseHp(bat->getDamage());
					//health -= bat->getDamage();

					if (isStandOnStair == false || health == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1)
								SetNx(-1);
							else
								SetNx(1);
						}
						SetState(simon_ani_hurt);
					}
				}
				else
				{
					if (e->ny != 0)
						y += dy;
					if (e->nx != 0)
						x += dx;
				}
			}
			//else if (dynamic_cast<Monkey*>(e->obj))
			//{
			//	if (untouchtime->IsTimeUp() && state != simon_ani_led && watertime->IsTimeUp())
			//	{
			//		untouchtime->Start();
			//		Monkey* monkey = dynamic_cast<Monkey*>(e->obj);
			//		loseHp(/*monkey->getDamage()*/0);
			//		if (isStandOnStair == false || health == 0)
			//		{
			//			if (e->nx != 0 || e->ny != 0)
			//			{
			//				if (e->nx == 1)
			//					SetNx(-1);
			//				else
			//					SetNx(1);
			//			}
			//			SetState(simon_ani_hurt);
			//		}
			//	}
			//	else
			//	{
			//		if (e->ny != 0)
			//			y += dy;
			//		if (e->nx != 0)
			//			x += dx;
			//	}
			//}
			/*else if (dynamic_cast<Frog*>(e->obj))
			{
				if (untouchtime->IsTimeUp() && state != simon_ani_led && watertime->IsTimeUp())
				{
					untouchtime->Start();
					Frog* frog = dynamic_cast<Frog*>(e->obj);
					loseHp(frog->getDamage());
					if (isStandOnStair == false || health == 0)
					{
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->nx == 1)
								SetNx(-1);
							else
								SetNx(1);
						}
						SetState(simon_ani_hurt);
					}
				}
				else
				{
					if (e->ny != 0)
						y += dy;
					if (e->nx != 0)
						x += dx;
				}
			}*/
			else if (dynamic_cast<Skeleton*>(e->obj))
			{
				if (untouchtime->IsTimeUp() && state != simon_ani_led && watertime->IsTimeUp())
				{
					untouchtime->Start();
					Skeleton* skele = dynamic_cast<Skeleton*>(e->obj);
					loseHp(skele->getDamage());
					if (isStandOnStair == false || health == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1)
								SetNx(-1);
							else
								SetNx(1);
						}
						SetState(simon_ani_hurt);
					}
				}
				else
				{
					if (e->ny != 0)
						y += dy;
					if (e->nx != 0)
						x += dx;
				}
			}
			else if (dynamic_cast<Raven*>(e->obj))
			{
				if (untouchtime->IsTimeUp() && state != simon_ani_led && watertime->IsTimeUp())
				{
					untouchtime->Start();
					Raven* raven = dynamic_cast<Raven*>(e->obj);
					//ravendie = true;
					raven->Setcollisimon(true);
					raven->SetState(raven_ani_die);
					loseHp(raven->getDamage());
					if (isStandOnStair == false || health == 0)
					{
						if (e->nx != 0 || e->ny != 0)
						{
							if (e->nx == 1)
								SetNx(-1);
							else
								SetNx(1);
						}
						SetState(simon_ani_hurt);
					}
				}
				else
				{
					if (e->ny != 0)
						y += dy;
					if (e->nx != 0)
						x += dx;
				}
			}
		}
	}
	

	for (UINT i = 0; i < coEvents.size(); i++) 
		delete coEvents[i];
}

void Simon::Render()
{
	int alpha = 255;
	if (!untouchtime->IsTimeUp())
		alpha = rand() % 255;
	if (!watertime->IsTimeUp() && state != simon_ani_led)
	{
		if ((float)(GetTickCount() - watertime->GetTimeStart()) < 1000)
			alpha = 70;
		else if (1000 <= (float)(GetTickCount() - watertime->GetTimeStart()) < 2500)
		{
			alpha = rand() % 76 + 100;
		}
		else
			alpha = rand() & 155 + 176;
	}
	animation_set->at(state)->Render(nx, x, y, alpha);
	
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
		if (!checkgroundmove)
			vx = 0;
		isStandOnStair = false;
		break;
	case simon_ani_jump:
		if (isGrounded)
		{
			if (vx != simon_run && vx != -simon_run)
				vx = 0;
			checkgroundmove = false;
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
		isGrounded = true;
		isStandOnStair = true;
		if (nx > 0) vx = simon_stair;
		else vx = -simon_stair;
		vy = -simon_stair;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_stair_down:
		isGrounded = true;
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
	case simon_ani_hurt:
		vy = -0.3;
		if (nx > 0) vx = -0.13;
		else vx = 0.13;
		animation_set->at(State)->ResetcurrentFrame();
		animation_set->at(State)->StartAni();
		break;
	case simon_ani_dead:
		untouchtime->Stop();
		watertime->Stop();
		vx = 0;
		vy = 1000;
		//life -= 1;
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

void Simon::SimonColliWithMob(vector<LPGAMEOBJECT> *listmob)
{
	float l_mob, t_mob, r_mob, b_mob, l_simon, t_simon, r_simon, b_simon;
	GetBoundingBox(l_simon, t_simon, r_simon, b_simon);

	for (UINT i = 0; i < listmob->size(); i++)
	{
		LPGAMEOBJECT e = listmob->at(i);
		if (dynamic_cast<Knight*>(e))
		{
			Knight* knight = dynamic_cast<Knight*>(e);
			knight->GetBoundingBox(l_mob, t_mob, r_mob, b_mob);
			if (CGameObject::AABBCheck(l_mob, t_mob, r_mob, b_mob, l_simon, t_simon, r_simon, b_simon) && untouchtime->IsTimeUp() && watertime->IsTimeUp()&& state != simon_ani_led)
			{
				untouchtime->Start();
				loseHp(knight->getDamage());
				if (isStandOnStair == false || health == 0)
				{
					if (e->nx != 0)
					{
						if (e->nx == 1)
							SetNx(-1);
						else
							SetNx(1);
					}

					SetState(simon_ani_hurt);
				}

			}

		}
		else if (dynamic_cast<Bat*>(e))
		{
			Bat* bat = dynamic_cast<Bat*>(e);
			bat->GetBoundingBox(l_mob, t_mob, r_mob, b_mob);
			if (CGameObject::AABBCheck(l_mob, t_mob, r_mob, b_mob, l_simon, t_simon, r_simon, b_simon) && untouchtime->IsTimeUp() && watertime->IsTimeUp() && state != simon_ani_led&&bat->GetState()!=bat_ani_die)
			{
				untouchtime->Start();
				loseHp(bat->getDamage());
				if (isStandOnStair == false || health == 0)
					SetState(simon_ani_hurt);
			}
		}
		else if (dynamic_cast<Frog*>(e))
		{
			Frog* frog = dynamic_cast<Frog*>(e);
			frog->GetBoundingBox(l_mob, t_mob, r_mob, b_mob);
			if (CGameObject::AABBCheck(l_mob, t_mob, r_mob, b_mob, l_simon, t_simon, r_simon, b_simon) && untouchtime->IsTimeUp() && watertime->IsTimeUp() && state != simon_ani_led)
			{
				untouchtime->Start();
				loseHp(/*frog->getDamage()*/0);
				if (isStandOnStair == false || health == 0)
				{
					if (e->nx != 0)
					{
						if (e->nx == 1)
							SetNx(-1);
						else
							SetNx(1);
					}
					SetState(simon_ani_hurt);
				}
			}
		}
		else if (dynamic_cast<Monkey*>(e))
		{
			Monkey* monkey = dynamic_cast<Monkey*>(e);
			monkey->GetBoundingBox(l_mob, t_mob, r_mob, b_mob);
			if (CGameObject::AABBCheck(l_mob, t_mob, r_mob, b_mob, l_simon, t_simon, r_simon, b_simon) && untouchtime->IsTimeUp() && watertime->IsTimeUp() && state != simon_ani_led)
			{
				untouchtime->Start();
				loseHp(/*monkey->getDamage()*/0);
				if (isStandOnStair == false || health == 0)
				{
					if (e->nx != 0)
					{
						if (e->nx == 1)
							SetNx(-1);
						else
							SetNx(1);
					}

					SetState(simon_ani_hurt);
				}
			}
		}
		else if (dynamic_cast<Skeleton*>(e))
		{
			Skeleton* skele = dynamic_cast<Skeleton*>(e);
			skele->GetBoundingBox(l_mob, t_mob, r_mob, b_mob);
			if (CGameObject::AABBCheck(l_mob, t_mob, r_mob, b_mob, l_simon, t_simon, r_simon, b_simon) && untouchtime->IsTimeUp() && watertime->IsTimeUp() && state != simon_ani_led&&skele->GetState()!=skeleton_ani_die)
			{
				untouchtime->Start();
				loseHp(skele->getDamage());
				if (isStandOnStair == false || health == 0)
					SetState(simon_ani_hurt);
			}
		}
		else if (dynamic_cast<Zombie*>(e))
		{
			Zombie* zombie = dynamic_cast<Zombie*>(e);
			zombie->GetBoundingBox(l_mob, t_mob, r_mob, b_mob);
			if (CGameObject::AABBCheck(l_mob, t_mob, r_mob, b_mob, l_simon, t_simon, r_simon, b_simon) && untouchtime->IsTimeUp() && watertime->IsTimeUp() && state != simon_ani_led && zombie->GetState() != zombie_ani_die)
			{
				untouchtime->Start();
				loseHp(zombie->getDamage());
				if (isStandOnStair == false || health == 0)
				{
					if (e->nx != 0)
					{
						if (e->nx == 1)
							SetNx(-1);
						else
							SetNx(1);
					}
					SetState(simon_ani_hurt);
				}
			}
		}
	}
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
			if (e->idItems == items_for_whip)
			{
				SetState(simon_ani_led);
				e->isDone = true;
				if (whip->GetState() == whip_lv1)
					whip->SetState(whip_lv2);
				else if (whip->GetState() == whip_lv2)
					whip->SetState(whip_lv3);
				

			}
			else if (e->idItems == items_big_heart)
			{
				
				e->isDone = true;
				//health = health - 1;
				mana += 5;
			}
			else if (e->idItems == items_knife)
			{
				InstallKnife();
				e->isDone = true;
			}
			else if (e->idItems == items_watch)
			{
				InstallClk();
				e->isDone = true;
				//currentWeapon = weapon_watch;
			}
			else if (e->idItems == items_axe)
			{
				InstallAxe();			
				e->isDone = true;
			}
			else if (e->idItems == items_boom)
			{
				InstallBoom();
				e->isDone = true;
			}
			else if (e->idItems == items_holywater)
			{
				InstallHoly();
				e->isDone = true;
			}
			else if (e->idItems == items_watterbottle)
			{
				e->isDone = true;
				watertime->Start();
			}
			else if (e->idItems == items_corss)
			{
				isCross = true;
				e->isDone = true;
			}
			else if (e->idItems == items_double)
			{
				hitDoubleTriple = 0;
				e->isDone = true;
			}
			else if (e->idItems == items_triple)
			{
				hitDoubleTriple = 1;
				e->isDone = true;
			}
			else if (e->idItems == items_meat)
			{
				e->isDone = true;
				health += 4;
				if (health >= simon_max_health)
					health = 16;
			}
			else if (e->idItems == items_small_heart)
			{
				e->isDone = true;
				mana += 1;
			}
			else if (e->idItems == items_bluemoney)
			{
				e->isDone = true;
				score += 500;
			}
			else if (e->idItems == items_redmoney)
			{
				e->isDone = true;
				score += 1000;
			}
			else if (e->idItems == items_whitemoney)
			{
				e->isDone = true;
				score += 1500;
			}
			else if (e->idItems == items_crown)
			{
				e->isDone = true;
				score += 2000;
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

			if ((b_simon) < b_stair - 18)  canmovedownstair = true;
			if (y >= t_stair - 32) canmoveupstair = true;

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
				y += 0.5f; // để đảm bảo simon sẽ va chạm với bậc thang 
			isWalkStair = false;
		}
	}
}

void Simon::InstallKnife()
{
	listsub.clear();
	currentWeapon = weapon_knfie;
	for (int i = 0; i < 3; i++)
		listsub.push_back(knife[i]);
}

void Simon::InstallAxe()
{
	listsub.clear();
	currentWeapon = 2;
	for (int i = 0; i < 3; i++)
		listsub.push_back(axe[i]);
}

void Simon::InstallBoom()
{
	listsub.clear();
	currentWeapon = 3;
	for (int i = 0; i < 3; i++)
		listsub.push_back(boom[i]);
}

void Simon::InstallHoly()
{
	listsub.clear();
	currentWeapon = 4;
	for (int i = 0; i < 3; i++)
		listsub.push_back(holywater[i]);
}

void Simon::InstallClk()
{
	listsub.clear();
	currentWeapon = weapon_watch;
	listsub.push_back(clk);
}
void Simon::StandOnStair()
{
	vx = vy = 0;
}

void Simon::loseHp(int x)
{
	health -= x;
	if (health <= 0)
		health = 0;
}
Simon::~Simon()
{

}
