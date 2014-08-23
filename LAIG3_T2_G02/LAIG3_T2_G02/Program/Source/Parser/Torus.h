#pragma once
#include "Primitive.h"
#include "SceneLoader.h"
using namespace std;
extern SceneLoader *scene;

class Torus:public Primitive{
	float inner;
	float outer;
	int slices;
	int loops;

public:
Torus(float _inner,float _outer,int _slices,int _loops)
	{
		inner=_inner;
		outer=_outer;
		slices=_slices;
		loops=_loops;
	}
void draw();
};