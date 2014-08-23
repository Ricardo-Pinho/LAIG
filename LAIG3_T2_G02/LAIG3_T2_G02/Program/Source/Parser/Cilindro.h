#pragma once
#include "Primitive.h"
#include "SceneLoader.h"
using namespace std;
extern SceneLoader *scene;

class Cilindro:public Primitive{
	float base;
	float top;
	float height;
	int slices;
	int stacks;
public:
Cilindro(float _base,float _top,float _height,int _slices, int _stacks)
	{
		base=_base;
		top=_top;
		height=_height;
		slices=_slices;
		stacks=_stacks;
	}
	void draw();
};

 