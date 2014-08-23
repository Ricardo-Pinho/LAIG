#pragma once
#include "Primitive.h"
#include "SceneLoader.h"
using namespace std;
extern SceneLoader *scene;
class Triangulo:public Primitive{
	float x1;
	float y1;
	float z1;
	
	float x2;
	float y2;
	float z2;

	float x3;
	float y3;
	float z3;

public:
Triangulo(float _x1,float _x2,float _x3,float _y1,float _y2,float _y3,float _z1,float _z2,float _z3)
	{
		x1=_x1;
		x2=_x2;
		x3=_x3;
		y1=_y1;
		y2=_y2;
		y3=_y3;
		z1=_z1;
		z2=_z2;
		z3=_z3;
	}

	void draw();
};