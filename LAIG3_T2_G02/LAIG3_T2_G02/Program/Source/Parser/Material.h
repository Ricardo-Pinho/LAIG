#pragma once
#include <string>
#include "RGBA.h"
#include <iostream>
using namespace std;
class Material{
public:
	
	float emr;
	float emg;
	float emb;
	float ema;
	float ambr;
	float ambg;
	float ambb;
	float amba;
	float difr;
	float difg;
	float difb;
	float difa;
	float spr;
	float spg;
	float spb;
	float spa;

	float shininess;
	Material(){};
	Material(float* em,float* amb, float* dif, float* sp, float sh){
		emr=em[0];
		emg=em[1];
		emb=em[2];
		ema=em[3];
		ambr=amb[0];
		ambg=amb[1];
		ambb=amb[2];
		amba=amb[3];
		difr=dif[0];
		difg=dif[1];
		difb=dif[2];
		difa=dif[3];
		spr=sp[0];
		spg=sp[1];
		spb=sp[2];
		spa=sp[3];
		shininess=sh;
	}
	float getShininess()
	{
		return shininess;
	}

};