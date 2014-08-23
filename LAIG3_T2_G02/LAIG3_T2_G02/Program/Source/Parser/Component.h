#pragma once

#include <vector>
#include "No.h"
#include "SceneLoader.h"
using namespace std;
extern SceneLoader *scene;


class Component:public No{

	bool hasref;
	float m0;
	float m1;
	float m2;
	float m3;
	float m4;
	float m5;
	float m6;
	float m7;
	float m8;
	float m9;
	float m10;
	float m11;
	float m12;
	float m13;
	float m14;
	float m15;
	vector<string> componentes;
	vector<string> primitivas;

	string transformacoes;
public:
	Component(float* matrix,vector<string> _prim,vector<string> _comp,string _transf, bool _hasref)
	{
		this->matrix=matrix;
		primitivas=_prim;
		componentes=_comp;
		transformacoes=_transf;
		hasref=_hasref;
	}
	void draw( );
	vector<string> getPrim(){return primitivas;};
	vector<string> getComp(){return componentes;};
	float* matrix;
	float* getMatrix(){
		//cout<<m0<<" "<<m1<<" "<<m2<<" "<<m3<<endl;
		//cout<<m4<<" "<<m5<<" "<<m6<<" "<<m7<<endl;
		//cout<<m8<<" "<<m9<<" "<<m10<<" "<<m11<<endl;
		//cout<<m12<<" "<<m13<<" "<<m14<<" "<<m15<<endl;
		/*float matrix[16]={m0,m1,m2,m3,
						  m4,m5,m6,m7,
						  m8,m9,m10,m11,
						  m12,m13,m14,m15};*/
		return matrix;
	}
	string getTrans(){return transformacoes;};
	bool getTref(){return hasref;};
};