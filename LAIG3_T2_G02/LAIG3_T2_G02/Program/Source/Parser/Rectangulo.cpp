#pragma once


#include "Rectangulo.h"
using namespace std;
void Rectangulo::draw()
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
				//printf("entra aki no rectangulo\n variáveis:\nx1:%.f,x2:%.f\ny1:%.f,y2:%.f\n",x1,x2,y1,y2);
				/*float v1x=x2-x1;
				float v1y=0;
				float v1z=0;
				float v2x=x3-x1;
				float v2y=y3-y1;
				float v2z=0;
				
				// Calculo do produto vetorial de v1 e v2 
				float nx = (v1y * v2z) - (v1z * v2y);
				float ny = (v1z * v2x) - (v1x * v2z);
				float nz = (v1x * v2y) - (v1y * v2x);*/
				glPushMatrix();
				//glEnable(GL_COLOR_MATERIAL);
				glBegin(GL_POLYGON);
				glNormal3d(0,0,1);
				float deltax= abs(x1-x2);
				float deltay= abs(y1-y2);
				float xtext=deltax/length_s;
				float ytext=deltay/length_t;
				//float max_length=deltax/length_s
				//Construir o poligono.
				glTexCoord2f(0.0,0.0);glVertex2f(x1,y1);
				glTexCoord2f(xtext,0.0);glVertex2f(x2,y1);
				glTexCoord2f(xtext,ytext);glVertex2f(x2,y2);
				glTexCoord2f(0.0,ytext);glVertex2f(x1,y2);
				glEnd();
				glPopMatrix();
			
	}