#pragma once

#include <vector>
using namespace std;


class Matrix{

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
public:
	Matrix(){};
	Matrix(float* matrix)
	{
		m0=matrix[0];
		m1=matrix[1];
		m2=matrix[2];
		m3=matrix[3];
		m4=matrix[4];
		m5=matrix[5];
		m6=matrix[6];
		m7=matrix[7];
		m8=matrix[8];
		m9=matrix[9];
		m10=matrix[10];
		m11=matrix[11];
		m12=matrix[12];
		m13=matrix[13];
		m14=matrix[14];
		m15=matrix[15];
	}
	float* getMatrix(){
		//cout<<m0<<" "<<m1<<" "<<m2<<" "<<m3<<endl;
		//cout<<m4<<" "<<m5<<" "<<m6<<" "<<m7<<endl;
		//cout<<m8<<" "<<m9<<" "<<m10<<" "<<m11<<endl;
		//cout<<m12<<" "<<m13<<" "<<m14<<" "<<m15<<endl;
		float matrix[16]={m0,m1,m2,m3,
						  m4,m5,m6,m7,
						  m8,m9,m10,m11,
						  m12,m13,m14,m15};
		return matrix;
	}
};