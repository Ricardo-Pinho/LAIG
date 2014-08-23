#pragma once
#include "RGBA.h"
#include <stdlib.h>

class Illumination{
public:
	float doublesided;
	float local;
	RGBA ambient;
	RGBA background;
	Illumination(){}
	Illumination(float ds,float loc,RGBA amb,RGBA back){
		doublesided=ds;
		local=loc;
		ambient=amb;
		background=back;

	}
	float* getAmbient(){
			float* result;
			result=(float*)malloc(4*sizeof(float*));
			result[0]=ambient.r;
			result[1]=ambient.g;
			result[2]=ambient.b;
			result[3]=ambient.a;
			
			return result;
	}
	float* getBackground(){
			float* result;
			result=(float*)malloc(4*sizeof(float*));
			result[0]=background.r;
			result[1]=background.g;
			result[2]=background.b;
			result[3]=background.a;
			
			return result;
	}
};