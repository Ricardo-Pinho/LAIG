#pragma once


#include "Component.h"
using namespace std;




void Component::draw(){
	bool change_texture=false;
	bool change_material=false;
	bool isinher=false;
	glPushMatrix();
	//Materiais
	if(material_id!="inherit")
	{
	if(material_id=="none") scene->applyMaterial();
	else{
		change_material=true;
		}
	}
	else{ 
		scene->applyMaterial(father_material_id);
		material_id=father_material_id;
		isinher=true;
		change_material=true;
	}
	//Texturas
	if(texture_id!="inherit")
	{
		if(texture_id=="none"){
			glDisable(GL_TEXTURE_2D);
			length_s=1;
			length_t=1;
		}
		else{
			change_texture=true;
			}
	}
	else{
		texture_id=father_texture_id;
		change_texture=true;
	}
	//Transformações
	if(getTref()==0) 
		{	
			float *m1=getMatrix();
			glMultMatrixf(m1);
		}
	else 
		{
			string matid=getTrans();
			if(matid!=""){
			float *m2=scene->map_matrizes[matid];
			glMultMatrixf(m2);
			}
			else{
					float *m1=getMatrix();
					glMultMatrixf(m1);
			}
		}
	//Verificação dos filhos
	for(unsigned int i=0;i<componentes.size();i++)
	{
		//caso mude de texturas, faz o bind
		if(change_texture&&texture_id!="none")
		{
			Textura texture=scene->applyTexture(texture_id);
	
			length_s=texture.length_s;
			length_t=texture.length_t;
		}
		if(change_material)
			{
				//cout<<"applying material "<<material_id<<" to "<<comp.first<<endl;
				scene->applyMaterial(material_id);
			}
		scene->map_componentes[componentes[i]]->length_s=length_s;
		scene->map_componentes[componentes[i]]->length_t=length_t;
		scene->map_componentes[componentes[i]]->father_material_id=material_id;
		scene->map_componentes[componentes[i]]->father_texture_id=texture_id;
		scene->map_componentes[componentes[i]]->draw();
		}
	for(unsigned int i=0;i<primitivas.size();i++)
	{
	//Desenho das primitivas
		if(change_texture&&texture_id!="none")
		{
			Textura texture=scene->applyTexture(texture_id);
			length_s=texture.length_s;
			length_t=texture.length_t;
			//cout<<"applying "<<texture_id<<" with "<<texture.second.length_s<<" from "<<endl;
		}
		if(change_material) scene->applyMaterial(material_id);
		scene->map_primitivas[primitivas[i]]->length_s=length_s;
		scene->map_primitivas[primitivas[i]]->length_t=length_t;
		scene->map_primitivas[primitivas[i]]->draw();
	}
	if(isinher) material_id="inherit";
	glPopMatrix();
	}
//buutton--translationz xy lateral