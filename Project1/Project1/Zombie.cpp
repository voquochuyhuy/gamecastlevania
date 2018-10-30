#include "Zombie.h"
#include "define.h"
void Zombie::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;

	if (state == ZOMBIE_STATE_DIE)
		bottom = y + ZOMBIE_BBOX_HEIGHT_DIE;
	else
		bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > SCREEN_WIDTH) {
		x = SCREEN_WIDTH; vx = -vx;
	}
}

void Zombie::Render()
{
	int ani = ZOMBIE_ANI_WALKING;
	if (state == ZOMBIE_STATE_DIE) {
		ani = ZOMBIE_ANI_DIE;
	}

	animations[ani]->Render(x, y);
	//RenderBoundingBox();
}

void Zombie::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ZOMBIE_STATE_DIE:
		y += ZOMBIE_BBOX_HEIGHT - ZOMBIE_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case ZOMBIE_STATE_WALKING:
		vx = -ZOMBIE_WALKING_SPEED;
	}

}

