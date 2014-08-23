#pragma once
#include "Primitive.h"
#include "SceneLoader.h"
using namespace std;
extern SceneLoader *scene;

class Esfera:public Primitive{
	float radius;
	int slices;
	int stacks;
public:
Esfera(float _radius,int _slices,int _stacks)
	{
		radius=_radius;
		slices=_slices;
		stacks=_stacks;
	}
void draw();
};