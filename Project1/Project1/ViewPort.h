#pragma once
#include <d3dx9.h>
#include "define.h"

class ViewPort
{
private:
	float x, y;
public:
	ViewPort(float x, float y);
	ViewPort();
	~ViewPort();

	
	

	
	void SetCameraPosition(float x, float y);

	
	float GetCameraPositionX();
	float GetCameraPositionY();
};



