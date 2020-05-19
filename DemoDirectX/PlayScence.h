#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Simon.h"
#include "Candle.h"
#include "Whip.h"
#include "Ground.h"
#include "Items.h"
#include "Knife.h"
#include "Hit.h"
#include "TileMap.h"
#include "Gate.h"
#include "Board.h"
#include "Stair.h"
#include "GroundMoving.h"
#include "Knight.h"
#include "Bat.h"
#include "Timer.h"
#include "Monkey.h"
#include "Skeleton.h"

class CPlayScene : public CScene
{
protected:
	Simon *simon;

	int idstage;
	int current_scene;
	
	TileMap *tilemap;
	Board *board;
	
	vector<LPGAMEOBJECT> objects;
	
	vector<LPGAMEOBJECT> listitems;
	
	vector<LPGAMEOBJECT> liststairleft;
	vector<LPGAMEOBJECT> liststairright;
	
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLERANIMATIONS(string line);
	void _ParseSection_CLERANIMATIONSSET(string line);



	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_LINKMAP(string line);
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);

	void _ParseSection_LINKOBJECTS(string line);
	void _ParseSection_INFOMAP(string line);

	vector<string> linkmap;

public:
	
	CPlayScene(/*int id, LPCWSTR filePath*/);

	virtual void ClearAll(LPCWSTR a);

	virtual void Load(LPCWSTR sceneFilePath);
	virtual void LoadObject();
	virtual void LoadMap(LPCWSTR map);
	virtual void LoadBaseObject();
	virtual void SwitchScene(int id);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	Items* DropItem(float x, float y,int id);
	bool CheckInCam(LPGAMEOBJECT a);
	void UseCross();
	void Revival();

	Timer* timecross = new Timer(500);
	Timer* timedeadsimon = new Timer(2000);

	vector<LPGAMEOBJECT> * GetListStairsLeft() { return &(liststairleft); }
	vector<LPGAMEOBJECT> * GetListStairsRight() { return &(liststairright); }
	
	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	void RunRight();
	void RunLeft();
	void Jump();
	void Idle();
	void Sit();
	void Hit();
	void Hit_SubWeapon();

	void Stair_Up();
	void Stair_Down();
	bool Simon_Stair_Stand();
	bool StairCollisionsDetectionLeft();
	bool StairCollisionsDetectionRight();
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

