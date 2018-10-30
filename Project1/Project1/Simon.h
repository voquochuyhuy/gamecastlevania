#pragma once
#include "GameObject.h"
#include"ViewPort.h"
#include"define.h"


class Simon : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	
public:
	Simon() : CGameObject()
	{
		level = SIMON_LEVEL_BIG;
		untouchable = 0;
	}
	ViewPort smviewport;
	float z, t;//vi tri trong camera
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetPositionInViewPort() {
		z = x - smviewport.GetCameraPositionX();
		t = y - smviewport.GetCameraPositionY();
		
	}
	void ViewPortInWorld() {
		smviewport.SetCameraPosition(this->dx, 640 + this->dx);
	}
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
