#pragma once
using namespace std;
class XYZ{
public:
	float x;
	float y;
	float z;
	float w;
	XYZ(){
	}
	XYZ(float _x,float _y,float _z){
		x=_x;
		y=_y;
		z=_z;
	}
	XYZ(float _x,float _y,float _z,float _w){
		x=_x;
		y=_y;
		z=_z;
		w=_w;
	}

};