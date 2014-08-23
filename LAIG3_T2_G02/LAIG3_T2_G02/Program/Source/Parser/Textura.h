#pragma once
#include <string>

using namespace std;
class Textura{
public:
	int counter;
	string filename;
	float length_s;
	float length_t;
	
	Textura(){};
	Textura(int _counter,string fn, float ls,float lt){
		 counter=_counter;
		 filename=fn;
		 length_s=ls;
		 length_t=lt;
	}


};