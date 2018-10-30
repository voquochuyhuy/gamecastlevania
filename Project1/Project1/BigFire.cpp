#include "BigFire.h"

void BigFire::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void BigFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BIGFIRE_BBOX_WIDTH;
	b = y + BIGFIRE_BBOX_HEIGHT;
}
