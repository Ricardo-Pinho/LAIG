#pragma once
#include <string>
#include "No.h"
using namespace std;

class Primitive:public No{
public:
string tipo;
float mat[1];
vector<string> prim;
vector<string> getPrim(){return prim;};
vector<string> getComp(){return prim;};
string getTrans(){return "";};
bool getTref(){return true;};
float* getMatrix(){return mat;};
//virtual void draw()=0;
};