#pragma once
#include"SubWeapon.h"
class Holywater :public SubWeapon
{
	bool isHolyWaterShattered;
	int holyWaterShatteredCounter;
public:
	Holywater();
	~Holywater();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Render();
	void collisionwith(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void SetV();
	void SetPosSubWeapon(D3DXVECTOR3 pos, bool isstanding);
	bool CheckPosKnife(float a);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void StartHolyWater();
	void SetState(int State);

};

