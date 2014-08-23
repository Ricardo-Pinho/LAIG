#pragma once


#include "Torus.h"
using namespace std;
void Torus::draw()
	{
		if(texture_id!="inherit"){
			if(texture_id=="none"){
				glDisable(GL_TEXTURE_2D);
			}
			else{
				pair<string,Textura> texture=scene->getTexture(texture_id);
				
				glEnable(GL_TEXTURE_2D);
				int tid=texture.second.counter;
				glBindTexture(GL_TEXTURE_2D, tid);
			}
		}
		if(material_id!="inherit"){
			if(material_id=="none"){
				scene->applyMaterial(); 
			}
			else{
				scene->applyMaterial(material_id);
			}
		}
		glPushMatrix();
		//glEnable(GL_COLOR_MATERIAL);
		glutSolidTorus(inner,outer,slices, loops);
		glPopMatrix();
	}