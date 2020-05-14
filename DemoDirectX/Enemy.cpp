#include "Enemy.h"

Enemy::Enemy()
{

}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
}

void Enemy::SetState(int State)
{
	CGameObject::SetState(State);
}

int Enemy::getHp()
{
	return hp;
}

void Enemy::loseHp(int x)
{
	hp -= x;
	if (hp <= 0)
		hp = 0;
}
Enemy::~Enemy()
{
}
