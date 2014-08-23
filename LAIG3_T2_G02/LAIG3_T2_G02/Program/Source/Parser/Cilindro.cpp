#pragma once


#include "Cilindro.h"
using namespace std;
void Cilindro::draw(){
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
		GLUquadric* glQ = gluNewQuadric();

		glPushMatrix();
		gluQuadricDrawStyle(glQ, GLU_FILL);
		gluQuadricTexture(glQ, 1);
		gluQuadricNormals(glQ, GLU_SMOOTH);
		//gluQuadricDrawstyle(glQ, GLU_FILL);
		gluCylinder(glQ, base, top, height, slices, stacks);
		glPopMatrix();

		glPushMatrix();
		glRotatef(180,0,1,0);
		gluDisk(glQ,0,base,slices,1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,0,height);
		gluDisk(glQ,0,top,slices,1);
		glPopMatrix();

		gluDeleteQuadric(glQ);

	}