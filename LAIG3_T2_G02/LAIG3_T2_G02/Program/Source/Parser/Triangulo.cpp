#pragma once


#include "Triangulo.h"
using namespace std;
void Triangulo::draw()
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
					float v1x=x2-x1;
				float v1y=y2-y1;
				float v1z=z2-z1;
				float dist1=sqrt((v1x*v1x)+(v1y*v1y)+(v1z*v1z));
				float repeatX=dist1/length_s;

				float v2x=x3-x1;
				float v2y=y3-y1;
				float v2z=z3-z1;
				float dist2=sqrt((v2x*v2x)+(v2y*v2y)+(v2z*v2z));
				
				float upper=v1x*v2x+v1y*v2y+v1z*v2z;
				float angle=acos(upper/(dist1*dist2));

				//float h=abs(sin(angle))*dist2;
				//float d=abs(cos(angle))*dist2;
				float h=sin(angle)*dist2;
				float d=cos(angle)*dist2;
				float repeatY=h/length_t;
				float repeat3=d/length_s;
				/* Calculo do produto vetorial de v1 e v2 */
				float nx = (v1y * v2z) - (v1z * v2y);
				float ny = (v1z * v2x) - (v1x * v2z);
				float nz = (v1x * v2y) - (v1y * v2x);
				glPushMatrix();
				//glEnable(GL_COLOR_MATERIAL);
				glBegin(GL_POLYGON);
				glNormal3d(nx,ny,nz);
				float deltax=abs(x2-x1);
				//Construir o poligono.
				glTexCoord2f(0.0,0.0);glVertex3d(x1,y1,z1);
				glTexCoord2f(repeatX,0.0);glVertex3d(x2,y2,z2);
				glTexCoord2f(repeat3,repeatY);glVertex3d(x3,y3,z3);
				glEnd();
				glPopMatrix();
	}