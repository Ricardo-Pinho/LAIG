#ifndef SCENE_LOADER
#define SCENE_LOADER

#include "tinyxml.h"
#include "gl\glui.h"
#include "gl\glut.h"
#include <map>
#include <string>
#include <vector>
class SceneLoader;
#include "No.h"
#include "Material.h"
#include "Textura.h"
#include "Light.h"
#include "Geometricas.h"
#include <iostream>
#include "Matrix.h"

#include "Component.h"
#include "Omni.h"
#include "Spot.h"
#include "View.h"
#include "../RGBpixmap.h"
#include "Illumination.h"
using namespace std;

// Apontadores para os principais grupos
// declarados aqui como globais por conveniência
// idealmente tudo seria incluído numa classe

struct point {
	float x;
	float y;
	float z;
};

class SceneLoader {

public:
	SceneLoader(const char * fileName);
	pair<string,No*> getIndependendcomponent();
	pair<string,No*> getComponent(string id);
	pair<string,No*> getPrimitive(string id);
	pair<string,Textura> getTexture(string id);
	pair<string,Material> getMaterial(string id);
	Textura applyTexture(string id);
	void applyMaterial(string id);
	void applyMaterial(void);
	int loadScene2(const char * fileName);
	float scene_length_axis;
	string root_id;

	void setNtheme(int i){ntheme=i;}; 

	//map<string, Matrix> map_matrizes;
	map<string, float*> map_matrizes;
	map<string, Textura >map_texturas;
	map<string, Material> map_materiais;
	map<string, No*> map_componentes;
	map<string, No*> map_primitivas;
	map<string, Light*> map_luzes;
	map<string, View*> map_vistas;

	Illumination illumination;
	string default_View;
	string current_View;
	RGBpixmap pixmap;

private:
  
	int ntheme;
	TiXmlElement *root;
	TiXmlDocument doc;

	int processGraphNode(TiXmlElement *parent, int nivel);

};

#endif
