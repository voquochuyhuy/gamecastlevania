#include "ViewPort.h"



ViewPort::ViewPort(float x, float y)
{
	this->x = x;
	this->y = y;

}
ViewPort::ViewPort()
{
}

ViewPort::~ViewPort()
{
}


void ViewPort::SetCameraPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

float ViewPort::GetCameraPositionX()
{
	return x;
}
float ViewPort::GetCameraPositionY()
{
	return y;
}

