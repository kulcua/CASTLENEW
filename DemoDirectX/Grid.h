#pragma once
#include"GameObject.h"
#include"Game.h"
class Grid
{
	int map_width, map_height;
	int cell_width, cell_height;
	vector<vector<vector<LPGAMEOBJECT>>> allcells;
public:
	Grid(/*int map_width,int map_height,*/int cell_width=265,int cell_height=250);
	~Grid();
	void Resize(int map_width, int map_height);
	void ClearGrid(int nums_row, int nums_col);
	void ResetGrid(vector<LPGAMEOBJECT> list);
	void PushGrid(vector<LPGAMEOBJECT> list);
	void GetGrid(vector<LPGAMEOBJECT> &list);
};

