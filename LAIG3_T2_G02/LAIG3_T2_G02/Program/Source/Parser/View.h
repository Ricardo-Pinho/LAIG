#pragma once
#include <string>

using namespace std;

class View{
public:
	string id;
	float near;
	float far;

	void setNear(float n){near=n;}
	void setFar(float f){far=f;}
	void setid(string i){id=i;}
	virtual void setView(void)=0;


};