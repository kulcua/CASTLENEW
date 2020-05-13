#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_START	7
#define SCENE_SECTION_LINKMAP	8
#define SCENE_SECTION_SCENEOBJECT	9
#define SCENE_SECTION_INFOMAP	10

#define SCENE_SECTION_CLEARTEXT	11
#define SCENE_SECTION_CLEARSP	12
#define SCENE_SECTION_CLEARANI	13
#define SCENE_SECTION_CLEARANISET	14



#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_GROUND	1
#define OBJECT_TYPE_CANDLE	2
#define OBJECT_TYPE_GATE	3
#define OBJECT_TYPE_STAIR	4
#define OBJECT_TYPE_GROUNDMOVING 5


#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


using namespace std;

CPlayScene::CPlayScene() :	CScene()
{
	key_handler = new CPlayScenceKeyHandler(this);
	//sceneFilePath = L"Scenes\\Castlevania.txt";
	Load(L"Scenes\\Castlevania.txt");//load ani sprites texture
	LoadBaseObject();
	SwitchScene(current_scene);

}





void CPlayScene::LoadBaseObject()
{
	if (simon == NULL)
	{
		simon = new Simon();
		DebugOut(L"[INFO]SIMON CREATED \n");
	}
	board = new Board(simon->GetHealth(), 16);
	tilemap = new TileMap();
}


void CPlayScene::SwitchScene(int idmap)
{
	
	Unload();
	if (idmap > 1)
		ClearAll(ToLPCWSTR(linkmap[idmap - 2]));
	CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
	LPCWSTR a = ToLPCWSTR(linkmap[idmap - 1]);	
	CGame::GetInstance()->SetCamPos(0.0f, 0.0f);
	Load(a);
	LoadMap(a);
	LoadObject();
	simon->currentscene = idmap;
}


Items* CPlayScene::DropItem(float x, float y,int id)
{
	Items *a = new Items();

	a->SetPosition(x, y);
	a->idItems = id;
	a->SetState(id);
	
	//listitems.push_back(a);
	return a;
}

void CPlayScene::_ParseSection_CLEARTEXTURES(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CTextures::GetInstance()->Clear(atoi(tokens[i].c_str()));
}

void CPlayScene::_ParseSection_CLEARSPRITES(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CSprites::GetInstance()->Clear(atoi(tokens[i].c_str()));
}

void CPlayScene::_ParseSection_CLERANIMATIONS(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CAnimations::GetInstance()->Clear(atoi(tokens[i].c_str()));
}

void CPlayScene::_ParseSection_CLERANIMATIONSSET(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CAnimationSets::GetInstance()->Clear(atoi(tokens[i].c_str()));
}

void CPlayScene::_ParseSection_LINKOBJECTS(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 1) return;

	sceneObject= ToLPCWSTR(tokens[0]);
}

void CPlayScene::ClearAll(LPCWSTR a)
{
	ifstream f;
	f.open(a);

	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;

		if (line == "[CLEARTEXT]") {
			section = SCENE_SECTION_CLEARTEXT; continue;
		}
		if (line == "[CLEARSP]") {
			section = SCENE_SECTION_CLEARSP; continue;
		}
		if (line == "[CLEARANI]") {
			section = SCENE_SECTION_CLEARANI; continue;
		}
		if (line == "[CLEARANISET]") {
			section = SCENE_SECTION_CLEARANISET; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }


		switch (section)
		{
		case SCENE_SECTION_CLEARTEXT:_ParseSection_CLEARTEXTURES(line); break;
		case SCENE_SECTION_CLEARSP:_ParseSection_CLEARSPRITES(line); break;
		case SCENE_SECTION_CLEARANI:_ParseSection_CLERANIMATIONS(line); break;
		case SCENE_SECTION_CLEARANISET:_ParseSection_CLERANIMATIONSSET(line); break;
		}
	}
	f.close();
}

void CPlayScene::_ParseSection_INFOMAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 10) return;
	int IDmap = atoi(tokens[0].c_str());
	wstring pathpic = ToWSTR(tokens[1]);
	wstring pathtxt = ToWSTR(tokens[2]);
	int num_row = atoi(tokens[3].c_str());
	int num_col = atoi(tokens[4].c_str());
	int num_row_read = atoi(tokens[5].c_str());
	int num_col_read = atoi(tokens[6].c_str());
	int tile_width = atoi(tokens[7].c_str());
	int tile_height = atoi(tokens[8].c_str());
	idstage = atoi(tokens[9].c_str());

	tilemap->LoadMap(IDmap, pathpic.c_str(), pathtxt.c_str(), num_row, num_col, num_row_read, num_col_read, tile_width, tile_height);
}

void CPlayScene::_ParseSection_LINKMAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 1) return;

	linkmap.push_back(tokens[0].c_str());
}

void CPlayScene::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}




void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	
	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
	{
		int nx1= atof(tokens[3].c_str());
		int state1= atof(tokens[4].c_str());
		float x2= atof(tokens[5].c_str());
		float y2 = atof(tokens[6].c_str());
		int nx2= atof(tokens[7].c_str());
		int state2 = atof(tokens[8].c_str());

		if (simon->currentscene < simon->nextscene)
		{
			simon->SetNx(nx1);
			simon->SetPosition(x, y);	
			simon->SetState(state1);
		}
		else if (simon->currentscene > simon->nextscene)
		{
			simon->SetPosition(x2, y2);
			simon->SetNx(nx2);
			simon->SetState(state2);
		}
		
		break;
	}
	case OBJECT_TYPE_GROUND:
	{
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		
		obj = new Ground();
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		objects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_CANDLE: 
	{
		int id = atof(tokens[4].c_str());
		obj = new Candle();

		obj->idItems = id;
		//allobject.push_back(obj);
		//objectsstatic.push_back(obj);
		obj->SetPosition(x, y);
		objects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		obj = new Gate();
		obj->nextscene = atof(tokens[3].c_str());
		/*current_scene = NEXTMAP;*/
		obj->SetPosition(x, y);
		objects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_STAIR:
	{
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj = new Stair();
		int stairnx = atof(tokens[4].c_str());
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		obj->stairdir = stairnx;
		if(stairnx==-1)
			liststairright.push_back(obj);
		else
			liststairleft.push_back(obj);
		break;
	}
	case OBJECT_TYPE_GROUNDMOVING:
	{
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj = new GroundMoving();
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		objects.push_back(obj);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
		break;
	}

	// General object setup
	
	/*obj->SetPosition(x, y);*/
}

void CPlayScene::LoadMap(LPCWSTR map)
{

	ifstream f;
	f.open(map);

	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;

		if (line == "[SCENEOBJECT]") {
			section = SCENE_SECTION_SCENEOBJECT; continue;
		}
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_INFOMAP; continue;
		}

		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }


		switch (section)
		{
		case SCENE_SECTION_SCENEOBJECT: _ParseSection_LINKOBJECTS(line); break;
		case SCENE_SECTION_INFOMAP:_ParseSection_INFOMAP(line); break;
		}
	}
	f.close();

}

void CPlayScene::LoadObject()
{

	DebugOut(L"[INFO] StartLOAD OBJECT  resources from : %s \n", sceneObject);

	ifstream f;
	f.open(sceneObject);

	/*if (f.fail())
	{
		DebugOut(L"[ERROR] LOAD OBJECT FAIL : %s \n", sceneObject);
		f.close();
		return;
	}*/

	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;

		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }


		switch (section)
		{
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Done LOAD OBJECT  resources from %s\n", sceneObject);
}

void CPlayScene::Load(LPCWSTR sceneFilePath)
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);


	/*if (f.fail())
	{
		DebugOut(L"[ERROR] LOAD ANISPRITETEXT FAIL : %s \n", sceneFilePath);
		f.close();
		return;
	}*/


	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[SETTINGS]") {
			section = SCENE_SECTION_START; continue;
		}
		if (line == "[LINKMAP]") {
			section = SCENE_SECTION_LINKMAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_START: _ParseSection_SETTINGS(line); break;
		case SCENE_SECTION_LINKMAP:  _ParseSection_LINKMAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	/*if ((simon->currentscene == 2 && simon->GetState() == 8))
	{
		simon->isGrounded = true;
		simon->isWalkStair = false;
	}*/

	/*if (simon->GetState() == 7)
	{
		simon->isGrounded = true;
	}*/

	

	vector<LPGAMEOBJECT> coObjects;
	

	for (int i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	


	for (UINT i = 0; i < objects.size()/*allobject.size()*/; i++)
	{
		LPGAMEOBJECT obj = objects[i]/*allobject[i]*/;
		
		if (dynamic_cast<Candle*>(obj) && obj->GetState() == break_candle && !(obj->isDone)&&!(obj->isFire))
		{
			
			if (obj->animation_set->at(break_candle)->RenderOver(time_render_fire))//để khi render xong lửa thì mới rới đồ
			{
				obj->isFire = true;
				listitems.push_back(DropItem(obj->GetPositionX(), obj->GetPositionY(), obj->idItems));
			}
			
		}
	}

	simon->SimonColliWithItems(&listitems);

	//for (int i = 0; i < objectsstatic.size(); i++) //update cây nến ở đây để cho khi render xong lửa thì mới rớt đồ
		//objectsstatic[i]->Update(dt);


	for (int i = 0; i < listitems.size(); i++)
		listitems[i]->Update(dt, &coObjects);


	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	simon->Update(dt, &coObjects);


	if (simon->GetState() == simon_ani_sit_hit)
		simon->GetWhip()->SetPosWhip(D3DXVECTOR3(simon->GetPositionX(), simon->GetPositionY(), 0), false);//false là ngồi
	else
		simon->GetWhip()->SetPosWhip(D3DXVECTOR3(simon->GetPositionX(), simon->GetPositionY(), 0), true);//true là đứng

	//for (int i = 0; i < coObjects.size(); i++)
	{
		if (simon->animation_set->at(simon_ani_stand_hit)->GetcurrentFrame() == 2 && simon->GetState() == simon_ani_stand_hit||(simon->animation_set->at(simon_ani_sit_hit)->GetcurrentFrame() == 2 && simon->GetState() == simon_ani_sit_hit)||(simon->animation_set->at(simon_ani_stair_up_hit)->GetcurrentFrame() == 2 && simon->GetState() == simon_ani_stair_up_hit))
		{
			if (simon->isHitSubWeapon)
			{				
				simon->GetKnife()->isDone = false;

				//simon->GetKnife()->SetNx(simon->Getnx());

				if (simon->GetState() == simon_ani_sit) //ko để dc trong update simon //để đây để có thể nhảy bắn
					simon->GetKnife()->SetPosSubWeapon(D3DXVECTOR3(simon->GetPositionX(), simon->GetPositionY(), 0), false);
				else
					simon->GetKnife()->SetPosSubWeapon(D3DXVECTOR3(simon->GetPositionX(), simon->GetPositionY(), 0), true);
			}
			else
				simon->GetWhip()->Update(dt, &objects/*&objectsstatic*/);
			
		}
	}

	if(simon->currentWeapon!=-1&&!simon->GetKnife()->isDone) // khác -1 để ko bay ra phi tiêu // ko thể thêm đk đang đánh vũ khí phụ dc
		simon->GetKnife()->Update(dt,&objects/*&objectsstatic*/);
		

	
	float cx, cy;
	simon->GetPosition(cx, cy);


	CGame *game = CGame::GetInstance();

	if (simon->isChangeScene)
	{	
		SwitchScene(simon->nextscene);
		simon->isChangeScene = false;	
	}


	//if (tilemap->getid() != 4000)
	{
		if (simon->GetPositionX() > (SCREEN_WIDTH / 2) && simon->GetPositionX() + (SCREEN_WIDTH / 2) < tilemap->getwidthmap())
		{
			cx = simon->GetPositionX() - (SCREEN_WIDTH / 2);
			CGame::GetInstance()->SetCamPos(cx, 0.0f );
		}
	}
	





	board->Update(dt, simon->GetHealth(), 16);
}

void CPlayScene::Render()
{
	
	board->Render(idstage, CGame::GetInstance()->GetCamPosX(), 0, simon);

	
	tilemap->Draw();
	
	for (int i = 0; i < liststairleft.size(); i++)
		liststairleft[i]->Render();


	for (int i = 0; i < liststairright.size(); i++)
		liststairright[i]->Render();

	for (int i = 0; i < listitems.size(); i++)
		listitems[i]->Render();

	/*for (int i = 0; i < objectsstatic.size(); i++)
		objectsstatic[i]->Render();*/

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();


	simon->Render();

	if (simon->Getnx() > 0)
		simon->GetWhip()->SetNx(1);
	else
		simon->GetWhip()->SetNx(-1);

	if ((simon->GetState() == simon_ani_stand_hit || simon->GetState() == simon_ani_sit_hit || simon->GetState() == simon_ani_stair_down_hit || simon->GetState() == simon_ani_stair_up_hit) && !simon->isHitSubWeapon || (simon->isHitSubWeapon&&simon->currentWeapon == -1))
	{
		simon->GetWhip()->Render(simon->animation_set->at(simon->GetState())->GetcurrentFrame());
	}
	else
		simon->GetWhip()->Render(-1);

	if (simon->currentWeapon != -1)
	{
		simon->GetKnife()->Render();
	}

	/*for (int i = 0; i < listHit.size(); i++)
		listHit[i]->Render();*/
	
	//if(simon->isHitSubWeapon&&simon->currentWeapon!=-1)
		//simon->GetKnife()->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();

	for (int i = 0; i < listitems.size(); i++)
		delete listitems[i];
	listitems.clear();

	for (int i = 0; i < liststairleft.size(); i++)
		delete liststairleft[i];
	liststairleft.clear();

	for (int i = 0; i < liststairright.size(); i++)
		delete liststairright[i];
	liststairright.clear();

	//simon = NULL;
}

void CPlayScenceKeyHandler::RunRight()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	simon->SetNx(1);
	simon->SetState(simon_ani_run);
}

void CPlayScenceKeyHandler::RunLeft()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	simon->SetNx(-1);
	simon->SetState(simon_ani_run);
	
}
// ,
void CPlayScenceKeyHandler::Jump()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	if (simon->GetState() == simon_ani_stand_hit || simon->GetState() == simon_ani_sit_hit || simon->GetState() == simon_ani_sit || simon->GetState() == simon_ani_led)
		return;

	if (simon->isGrounded)
		simon->SetState(simon_ani_jump);
}

void CPlayScenceKeyHandler::Idle()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	if (simon->isGrounded)
		simon->SetState(simon_ani_idle);
}

void CPlayScenceKeyHandler::Hit()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	if ((simon->GetState() == simon_ani_idle || simon->GetState() == simon_ani_jump || simon->GetState() == simon_ani_run))
	{
		if (simon->isGrounded)
		{
			simon->SetState(simon_ani_stand_hit);
			simon->SetVx(0);
		}
		else
		{
			simon->SetState(simon_ani_stand_hit);
		}
	}
	else if (simon->GetState() == simon_ani_sit)
		simon->SetState(simon_ani_sit_hit);
	else if (simon->GetState() == simon_ani_stair_up)
		simon->SetState(simon_ani_stair_up_hit);
	else if (simon->GetState() == simon_ani_stair_down)
		simon->SetState(simon_ani_stair_down_hit);
}

void CPlayScenceKeyHandler::Sit()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	simon->SetState(simon_ani_sit);
}

void CPlayScenceKeyHandler::Hit_SubWeapon()
{
	Simon *simon = ((CPlayScene*)scence)->simon;

	if (simon->currentWeapon == -1||simon->getmana()==0)
	{
		Hit();
		return;  //return để không bị đánh khi không có vũ khí phụ
	}

	if (simon->GetKnife()->isDone == false) //để cho vũ khí phụ ko thể đánh quá nhiều
		return;
	simon->isHitSubWeapon = true;
	simon->GetKnife()->SetNx(simon->Getnx());//để hướng ở đây để ko bị thay đổi khi simon quay lưng ngay lập tức
	if (simon->getcurrentweapon() == 0)
	{
		simon->GetKnife()->SetState(knife_ani);
		simon->usemana(1);
	}
	Hit();
}

void CPlayScenceKeyHandler::Stair_Down()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	int Stairnx = simon->stairNx; //hướng cầu thang

	if (!simon->canmovedownstair)//ra khỏi cầu thang
	{
		if (simon->isStandOnStair)
			simon->SetState(simon_ani_idle);
		else
			simon->SetState(simon_ani_sit);
		return;
	}
	
	if (!simon->isStandOnStair)// đi đúng đến vị trí cần đi
	{
		float stairx, simonx;
		stairx =simon->stairCollided->GetPositionX();
		simonx = simon->GetPositionX();
		if (Stairnx == 1)
			stairx += 4.0f;
		else 
			stairx -= 31.0f;

		if (stairx < simonx) simon->SetNx(-1);
		else if (stairx > simonx) simon->SetNx(1);
		simon->vy = 0;
		simon->SetState(simon_ani_run);
		simon->AutoWalkStair(stairx, simon_ani_stair_down, -Stairnx);
		simon->isStandOnStair = true;
		return;
	}
	else
	{
		simon->nx = -simon->stairNx;
		simon->SetState(simon_ani_stair_down);

	}
	return;
}

void CPlayScenceKeyHandler::Stair_Up()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	int NxStair = simon->stairNx;

	if (!simon->canmoveupstair) 
	{
		if (simon->isStandOnStair)
		{
			float stairx;
			stairx = simon->stairCollided->GetPositionX();
			if (NxStair == 1)
				stairx += 5.0f;
			else 
				stairx -= 30.0f;
			simon->nx = simon->stairNx;
			simon->SetState(simon_ani_stair_up);
			simon->AutoWalkStair(stairx, simon_ani_idle, simon->nx);//tránh trường hợp ra khỏi cầu thang mà ko dụng vào mặt đất
		}
		return;
	}

	

	if (!simon->isStandOnStair) // đi đúng đến vị trí cần đi
	{
		float stair_x, simon_x;
		stair_x = simon->stairCollided->GetPositionX();
		simon_x = simon->GetPositionX();
		
		
		if (NxStair == 1)
			stair_x -= 31.0f;
		else
			stair_x += 4.0f;

		if (stair_x < simon_x) simon->SetNx(-1);
		else if (stair_x > simon_x)  simon->SetNx(1);
		simon->SetState(simon_ani_run);
		simon->vy = 0;
		simon->AutoWalkStair(stair_x, simon_ani_stair_up, NxStair);
		simon->isStandOnStair = true;
		return;
	}
	else
	{
		simon->SetNx(NxStair);
		simon->SetState(simon_ani_stair_up);
	}
	return;
}

bool CPlayScenceKeyHandler::Simon_Stair_Stand()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	if (simon->GetState() == simon_ani_stair_up || simon->GetState() == simon_ani_stair_down || simon->GetState() == simon_ani_stair_down_hit || simon->GetState() == simon_ani_stair_up_hit)
	{
		if (simon->GetState() == simon_ani_stair_up_hit)
		{
			simon->SetState(simon_ani_stair_up);
			simon->animation_set->at(simon_ani_stair_up)->startFrameTime = 0;
		}
		else if (simon->GetState() == simon_ani_stair_down_hit)
		{
			simon->SetState(simon_ani_stair_down);
			simon->animation_set->at(simon_ani_stair_down)->startFrameTime = 0;
		}

		simon->StandOnStair();
		simon->animation_set->at(simon_ani_stair_up)->ResetcurrentFrame();
		simon->animation_set->at(simon_ani_stair_down)->ResetcurrentFrame();

		return true;
	}

	return false;
}

bool CPlayScenceKeyHandler::StairCollisionsDetectionLeft()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	CPlayScene* playscene = ((CPlayScene*)scence);
	simon->StairLeftFirst = true;
	return	simon->SimonColliWithStair(playscene->GetListStairsLeft());
}

bool CPlayScenceKeyHandler::StairCollisionsDetectionRight()
{
	Simon *simon = ((CPlayScene*)scence)->simon;
	CPlayScene* playscene = ((CPlayScene*)scence);
	return	simon->SimonColliWithStair(playscene->GetListStairsRight());
}


void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame *game = CGame::GetInstance();
	Simon *simon = ((CPlayScene*)scence)->simon;
	switch (KeyCode)
	{
	case DIK_A:
		if (game->IsKeyDown(DIK_UP))
			Hit_SubWeapon();
		else
			Hit();
		break;
	case DIK_S:
		if (simon->isStandOnStair == false)
			Jump();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	Simon *simon = ((CPlayScene*)scence)->simon;

	
	if (simon->isWalkStair == true)
		return;

	if (simon->GetState() == simon_ani_stand_hit && simon->animation_set->at(simon_ani_stand_hit)->RenderOver(simon_delay_hit))//để cho ko bị đánh 2 lần
	{
		if (!(simon->isGrounded))
			simon->SetState(simon_ani_jump);
		else
			simon->SetState(simon_ani_idle);
	}

	if (!(simon->isGrounded))
		return;

	if (simon->GetState() == simon_ani_stair_up_hit && !(simon->animation_set->at(simon_ani_stair_up_hit)->RenderOver(300)))
		return;

	if (simon->GetState() == simon_ani_stair_down_hit && !(simon->animation_set->at(simon_ani_stair_down_hit)->RenderOver(300)))
		return;

	if (simon->GetState() == simon_ani_stair_up && !(simon->animation_set->at(simon_ani_stair_up)->RenderOver(200)))
		return;

	if (simon->GetState() == simon_ani_stair_down && !(simon->animation_set->at(simon_ani_stair_down)->RenderOver(200)))
		return;
	
	if (simon->GetState() == simon_ani_led && !(simon->animation_set->at(simon_ani_led)->RenderOver(simon_delay_led)))
		return;

	if ((simon->GetState() == simon_ani_stand_hit && !(simon->animation_set->at(simon_ani_stand_hit)->RenderOver(simon_delay_hit))) || (simon->GetState() == simon_ani_sit_hit && !simon->animation_set->at(simon_ani_sit_hit)->RenderOver(simon_delay_hit)))
	{	
		return;
	}
	else
		simon->isHitSubWeapon = false;





	if (game->IsKeyDown(DIK_DOWN))
	{
		if (StairCollisionsDetectionRight() && !simon->StairLeftFirst)
		{
			Stair_Down();
			return;
		}

		if (StairCollisionsDetectionLeft())
		{
			Stair_Down();
			return;
		}


		Sit();
		if (game->IsKeyDown(DIK_RIGHT))
			simon->SetNx(1);
		else if (game->IsKeyDown(DIK_LEFT))
			simon->SetNx(-1);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (StairCollisionsDetectionLeft())
		{
			simon->StairLeftFirst = false;
			Stair_Up();
			return;
		}

		if (StairCollisionsDetectionRight())
		{
			simon->StairLeftFirst = false;
			Stair_Up();
			return;
		}
		Idle();
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		RunLeft();
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		RunRight();
	}
	else
	{
		if (StairCollisionsDetectionRight() || StairCollisionsDetectionLeft())
		{
			if (Simon_Stair_Stand())
			{
				return;

			}
		}
		Idle();
	}
}