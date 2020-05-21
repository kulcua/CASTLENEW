#pragma once
#include"SubWeapon.h"
class Holywater :public SubWeapon
{
	bool isHolyWaterShattered = false;
	int holyWaterShatteredCounter = 0;
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
	void StartHolyWater() { isHolyWaterShattered = true; holyWaterShatteredCounter = GetTickCount(); }
	void SetState(int State);

};

