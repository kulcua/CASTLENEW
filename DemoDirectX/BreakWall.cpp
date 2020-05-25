#include "BreakWall.h"


BreakWall::BreakWall()
{
	isDone = isFire = false;
}



void BreakWall::Render()
{
	if (!isDone)
		animation_set->at(state)->Render(-1, x, y);
	else
		return;
}



void BreakWall::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (!isDone)
	{
		left = x;
		top = y;
		right = left + 32;
		bottom = top + 32;
	}
}

BreakWall::~BreakWall()
{
}
