#pragma once
#include "Light.h"
using namespace std;

class Omni: public Light{
public:
	Omni(string i):Light(i){
		setType("omni");
	}
	Omni(string _id,	bool _enabled,	XYZ _location, 	RGBA _ambient,  	RGBA _diffuse, 	RGBA _specular):Light( _id,	 _enabled,	 _location, _ambient, _diffuse, _specular){
		setType("omni");
	}

	float* calcDirection(){
		cout <<"acesso indevido"<<endl;
		return 0;
	}
	float getExponent(){
		cout <<"acesso indevido"<<endl;
		return 0;
	}
	float getAngle(){
	cout <<"acesso indevido"<<endl;
		return 0;
	}
	void makeLight(int light_number)
	{
		 GLUquadric* glQ = gluNewQuadric();
				// declarações para a fonte de luz LIGHT0;
				//float * light_pos;
				// float light0_position =light_pos[0];
				 float x=location.x;
				 float y=location.y;
				 float z=location.z;
				 float w=location.w;
				float light0_position[]  = {x, y, z, w}; // nao necessaria...

				 float * light_amb;
				 light_amb=getAmbient();
				 float* light0_ambient=light_amb;
		

				 float * light_diff;
				 light_diff=getDiffuse();
				 float* light0_diffuse=light_diff;

				  float * light_spec;
				 light_spec= getSpecular();
				 float* light0_specular=light_spec;
				  
	
				double light0x = x;
				double light0y = y;
				double light0z = z;

				// Actualizacao da posicao da fonte de luz...
				light0_position[0] = light0x;	// por razoes de eficiencia, os restantes 
				light0_position[1] = light0y;	// parametros _invariaveis_ da LIGHT0 mantem os valores
				light0_position[2] = light0z;	// definidos na funcao de inicializacao
				glLightfv(GL_LIGHT0+light_number, GL_POSITION, light0_position);
				// declaracoes para a fonte luz GL_LIGHT0
				glLightfv(GL_LIGHT0+light_number, GL_AMBIENT, light0_ambient);
				glLightfv(GL_LIGHT0+light_number, GL_DIFFUSE, light0_diffuse);
				glLightfv(GL_LIGHT0+light_number, GL_SPECULAR, light0_specular);


				gluDeleteQuadric(glQ);
				glEnable(GL_LIGHT0+light_number);
	}
};