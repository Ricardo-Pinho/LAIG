
#pragma once
#include <string>
#include "XYZ.h"
#include "RGBA.h"
using namespace std;

class Light{
public:
	string id;
	string type;
	int enabled;
	XYZ location;
	RGBA ambient;
	RGBA diffuse;
	RGBA specular;

	Light(string i){
		id=i;
	}
	Light(string _id,	int _enabled,	XYZ _location, 	RGBA _ambient,  	RGBA _diffuse, 	RGBA _specular){
		 id=_id;
		 enabled=_enabled;
		 location=_location;
		 ambient=_ambient; 
		 diffuse=_diffuse; 
		 specular=_specular;
	}
	void setId(string i){
		id=i;
	}
	void setEnable(bool en){
		enabled=en;
	}
	void setLocation(XYZ loc){
		location=loc;
	}
	void setAmbient(RGBA amb){
		ambient=amb;
	}
	void setDiffuse(RGBA dif){
		diffuse=dif;
	}
	void setSpecular(RGBA spec){
		specular=spec;
	}
	void setType(string t){
		type=t;
	}
	float* getAmbient(){
			
		float* result;
		result=(float*)malloc(16*sizeof(float*));
		result[0]=ambient.r;
		result[1]=ambient.g;
		result[2]=ambient.b;
		result[3]=ambient.a;
		return result;
	}
	float* getDiffuse(){
			
		float* result;
		result=(float*)malloc(16*sizeof(float*));
		result[0]=diffuse.r;
		result[1]=diffuse.g;
		result[2]=diffuse.b;
		result[3]=diffuse.a;
		return result;
	}
	float* getSpecular(){
			
		float* result;
		result=(float*)malloc(16*sizeof(float*));
		result[0]=specular.r;
		result[1]=specular.g;
		result[2]=specular.b;
		result[3]=specular.a;
		return result;
	}
	virtual float* calcDirection()=0;
	virtual float getExponent()=0;
	virtual float getAngle()=0;
	virtual void makeLight(int light_number)=0;
};