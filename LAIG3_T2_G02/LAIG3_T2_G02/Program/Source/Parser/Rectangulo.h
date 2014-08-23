#pragma once
#include "Primitive.h"
#include "SceneLoader.h"
using namespace std;
extern SceneLoader *scene;

class Rectangulo:public Primitive{
	float x1;
	float x2;
	float y1;
	float y2;
public:
	Rectangulo(float _x1,float _x2,float _y1,float _y2)
	{
		x1=_x1;
		x2=_x2;
		y1=_y1;
		y2=_y2;
	}
	void draw();
};