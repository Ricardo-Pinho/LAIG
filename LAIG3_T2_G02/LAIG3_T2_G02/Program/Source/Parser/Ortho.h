#pragma once
#include <string>
#include "XYZ.h"
#include "View.h"
using namespace std;

class Ortho:public View{
public:
	float left;
	float right;
	float top;
	float bottom;

	Ortho(float _left, float _right, float _top, float _bottom)
	{
		left=_left;
		right=_right;
		top=_top;
		bottom=_bottom;
				cout<<_bottom<<endl;
	}
	void setDir(float l,float r,float t,float b){
	left=l;
	right=r;
	t=top;
	b=bottom;}

	void setView()
	{
		//cout<<left<<" "<<right<<" "<<bottom<<" "<<top<<" "<<near<<" "<<far<<endl;
		glOrtho(left, right, bottom, top, near, far);
	}
	

};