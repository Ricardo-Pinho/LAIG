
#pragma once
#include <string>
#include <vector>
#include "gl\glui.h"
#include "gl\glut.h"
using namespace std;

class No{
public:
	string id;
	string material_id;
	string texture_id;
	string father_material_id;
	string father_texture_id;
	vector<string> materials;
	char key;
	float length_s;
	float length_t;
	virtual void draw()=0;
	virtual vector<string> getPrim(void)=0;
	virtual vector<string> getComp(void)=0;
	virtual float* getMatrix(void)=0;
	virtual string getTrans(void)=0;
	virtual bool getTref()=0;
	void setMaterial(string m){
		material_id=m;
	}
	void setTexture(string t){
		texture_id=t;
	}
	void setMaterialvector(vector<string> mat)
	{
		materials=mat;
	}
	void setKey(char _key)
	{
		key=_key;
	}

};