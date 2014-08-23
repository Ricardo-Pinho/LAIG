#pragma once
#include <string>
#include "XYZ.h"
#include "View.h"
using namespace std;

class Prespective:public View{
public:
	float angle;
	float fromx;
	float fromy;
	float fromz;
	float tox;
	float toy;
	float toz;
	Prespective(float _angle,XYZ _from, XYZ _to)
	{
		angle=_angle;
		fromx=_from.x;
		fromy=_from.y;
		fromz=_from.z;
		tox=_to.x;
		toy=_to.y;
		toz=_to.z;
	}
	void setAngle(float ang){angle=ang;}

	void setView()
	{
		float deltax=tox-fromx;
		float deltay=abs(toy-fromy);
		float deltaz=toz-fromz;
		float ratio=fromx/fromy;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(angle,1,near,far);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(fromx,fromy,fromz,tox,toy,toz,deltax,deltay,deltaz);
	
	}

};