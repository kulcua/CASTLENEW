#pragma once
#include"SubWeapon.h"
#include"Simon.h"
class Boomerang :public SubWeapon
{
	bool checkdamage1, checkdamage2;
public:
	Boomerang(LPGAMEOBJECT simon);
	~Boomerang();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Render();
	void collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void SetV();
	void SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding);
	bool CheckPosKnife(float a);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

