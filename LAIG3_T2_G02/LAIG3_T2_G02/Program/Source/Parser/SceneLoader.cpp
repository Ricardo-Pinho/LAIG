#include "SceneLoader.h"
#include "Geometricas.h"
#include "Prespective.h"
#include "Ortho.h"
#include <iostream>
using namespace std;

SceneLoader::SceneLoader(const char * fileName):
  doc(fileName)
{
  // Read string from file
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
	  // fazer subclassing à classe exception para dar mais informações...
	  cout<<"File Not loaded!"<<endl;
	  system("PAUSE");
	  exit(1);
	}
	
	this->root = doc.RootElement();
	scene_length_axis=10.0;
}




pair<string,No*> SceneLoader::getIndependendcomponent()
{
	map<string, No*>::iterator it;
	map<string, No*>::iterator ite;
	string id;
	bool independent;
	//Percorre todos os poligonos existentes no map de poligonos, na SceneLoader.
	for ( it = map_componentes.begin() ; it != map_componentes.end(); it++ ) 
	{
			 id=(*it).first;
			 independent=true;
			 for ( ite = map_componentes.begin() ; ite != map_componentes.end(); ite++ ) 
			{
				vector<string> newcomp=(*ite).second->getComp();
				for(unsigned int i=0;i<newcomp.size();i++)
				{
					if(newcomp[i]==id) {
						//cout<<id<<" is false because it is called in "<<(*ite).first<<endl;
						independent=false;
					}
				}
			}
			 if(independent==true&&id==root_id) {
				 //cout<<"component "<<id<<" is the root"<<endl; 
				 return (*it);
			 }
	}
	return (*it);
}

pair<string,No*> SceneLoader::getComponent(string id)
{
	map<string, No*>::iterator it;
	for ( it = map_componentes.begin() ; it != map_componentes.end(); it++ ) 
	{
		if(id==(*it).first){
			//cout<<"found Component with id "<<(*it).first<<" which is the same as "<<id<<" which is received"<<endl; 
			return (*it);
		}
	}
	return (*it);
}

pair<string,No*> SceneLoader::getPrimitive(string id)
{
	map<string, No*>::iterator it;
	for ( it = map_primitivas.begin() ; it != map_primitivas.end(); it++ ) 
	{
		if(id==(*it).first){
			//cout<<"found Component with id "<<(*it).first<<" which is the same as "<<id<<" which is received"<<endl; 
			return (*it);
		}
	}
	return (*it);
}

pair<string,Textura> SceneLoader::getTexture(string id)
{
	map<string, Textura>::iterator it;
	for ( it = map_texturas.begin() ; it != map_texturas.end(); it++ ) 
	{
		if(id==(*it).first){
			//cout<<"found Component with id "<<(*it).first<<" which is the same as "<<id<<" which is received"<<endl; 
			return (*it);
		}
	}
	return (*it);
}

pair<string,Material> SceneLoader::getMaterial(string id)
{
	map<string, Material>::iterator it;
	for ( it = map_materiais.begin() ; it != map_materiais.end(); it++ ) 
	{
		if(id==(*it).first){
			return (*it);
		}
	}
	return (*it);
}

Textura SceneLoader::applyTexture(string id)
{
			
			Textura texture=map_texturas[id];
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_2D);
			int tid=texture.counter+ntheme*100;
			glBindTexture(GL_TEXTURE_2D, tid);
			return texture;
}

void SceneLoader::applyMaterial(string id)
{
Material mat= map_materiais[id];
float mat1_emission[] = {mat.emr, mat.emg, mat.emb, mat.ema};
float mat1_specular[] = {mat.spr, mat.spg, mat.spb, mat.spa};	/* specular reflection. */
float mat1_diffuse[] =  {mat.difr, mat.difg, mat.difb, mat.difa};	/* diffuse reflection. */
float mat1_ambient[] =  {mat.ambr, mat.ambg, mat.ambb, mat.amba};	/* ambient reflection. */
float mat1_shininess[] =  {mat.shininess};

//cout<<id<<" is giving value "<<material.second.emr<<endl;//" "<<material.second.difg<<" "<<material.second.difb<<" "<<material.second.difa<<endl;

glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
glMaterialfv(GL_FRONT, GL_EMISSION,  mat1_emission);
glMaterialfv(GL_FRONT, GL_SPECULAR,  mat1_specular);
glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat1_diffuse);
glMaterialfv(GL_FRONT, GL_AMBIENT,   mat1_ambient);
}
void SceneLoader::applyMaterial()
{

float mat1_emission[] = {0.0, 0.0, 0.0, 1.0};
float mat1_specular[] = {0.4, 0.4, 0.4, 1.0};	/* specular reflection. */
float mat1_diffuse[] =  {0.7, 0.7, 0.7, 1.0};	/* diffuse reflection. */
float mat1_ambient[] =  {0.7, 0.7, 0.7, 1.0};	/* ambient reflection. */
float mat1_shininess[] =  {180.0};

glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
glMaterialfv(GL_FRONT, GL_EMISSION,  mat1_emission);
glMaterialfv(GL_FRONT, GL_SPECULAR,  mat1_specular);
glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat1_diffuse);
glMaterialfv(GL_FRONT, GL_AMBIENT,   mat1_ambient);
}

//Desenhar o Grafo



//-------------------------------------------------------

//----------------//
//----------------//
//   DO EXEMPLO   //
//----------------//
//----------------//



// Apontadores para os principais grupos
// declarados aqui como globais por conveniência
// idealmente tudo seria incluído numa classe

TiXmlElement* rootElement=NULL; 
TiXmlElement* matsElement=NULL;
TiXmlElement* textsElement=NULL;
TiXmlElement* viewsElement=NULL;
TiXmlElement* ilumElement=NULL;
TiXmlElement* graphElement=NULL;
TiXmlElement* primElement =NULL;
TiXmlElement* compElement =NULL; 
TiXmlElement* transElement =NULL;

TiXmlElement* lightsElement =NULL;


//-------------------------------------------------------

TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
// Função de pesquisa de um nó filho por comparação de um atributo (normalmente um id) com um valor de referência
// numa versão mais alto nível seria utilizada uma expressão XPath
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}

//------------------------------------------------------
//Para certificar que aquele id ainda nao existe. Retorna true se existir
template<class T>
bool findId(map<string,T> vec, string id){
	map<string,T>::iterator it;
	for(it=vec.begin();it!=vec.end();it++){
		if(it->first==id)
			return true;
	}
	return false;
}


//-------------------------------------------------------

int SceneLoader::processGraphNode(TiXmlElement *parent, int nivel)
// função recursiva de processamento do grafo
// ao encontrar um nó, aplica as definições declaradas e itera sobre os seus descendentes
// nota: se um nó A, declarado em 'Nodes' aparecer instanciado uma vez no grafo com descendentes B e C
// e posteriormente o nó A for novamente instanciado, a relação de descendência com B e C não é implícita.
// Neste modelo o nó é só uma transformação geométrica ou de atributos, não uma definição de um sub-grafo 
// com vários nós que pode ser reinstanciado
{
	//cout <<"ENTREI"<<endl;
	if(parent==NULL)
		cout<<"Parent is Null"<<endl;

	TiXmlElement *child=parent->FirstChildElement();
	//cout <<"ENTREI2"<<endl;
	// cria um prefixo de espaços para indentação, apenas para visualização
	int prefixLen=nivel*2;
	char prefix[100];
	memset(prefix,' ',prefixLen);
	prefix[prefixLen]=0;
	//ILLUMINATION
	while (child)
	{
		if (strcmp(parent->Value(),"illumination")==0){
				bool doublesided=atoi(parent->Attribute("doublesided"));
				bool local=atoi(parent->Attribute("local"));
				RGBA amb1;
				RGBA back1;
				TiXmlElement *Pchild=parent->FirstChildElement();
				int counter=0;
				while(Pchild){
				counter++;
					if(counter>2){
						cout<<"Demasiados childs"<<endl;
						return -1;
					}
					if(strcmp(Pchild->Value(),"ambient")==0){
						float amb_r;
						float amb_g;
						float amb_b;
						float amb_a;
						if(Pchild->QueryFloatAttribute("r",&amb_r)== TIXML_SUCCESS &&	Pchild->QueryFloatAttribute("g",&amb_g)== TIXML_SUCCESS &&
							Pchild->QueryFloatAttribute("b",&amb_b)== TIXML_SUCCESS &&Pchild->QueryFloatAttribute("a",&amb_a)== TIXML_SUCCESS){
							amb1.r=amb_r;
							amb1.g=amb_g;
							amb1.b=amb_b;
							amb1.a=amb_a;
						}
						else{
							cout<<"error reading Illumination"<<endl;
							return -1;
						}
					}
					else if(strcmp(Pchild->Value(),"background")==0){
						float back_r;
						float back_g;
						float back_b;
						float back_a;

						if(Pchild->QueryFloatAttribute("r",&back_r)== TIXML_SUCCESS &&	Pchild->QueryFloatAttribute("g",&back_g)== TIXML_SUCCESS &&
							Pchild->QueryFloatAttribute("b",&back_b)== TIXML_SUCCESS &&Pchild->QueryFloatAttribute("a",&back_a)== TIXML_SUCCESS){
							back1.r=back_r;
							back1.g=back_g;
							back1.b=back_b;
							back1.a=back_a;
						}
						else{
							cout<<"error reading Illumination"<<endl;
							return -1;
						}
					}
					Pchild=Pchild->NextSiblingElement();
				}
				Illumination ill(doublesided,local,amb1,back1);
				illumination=ill;
		}

		//VIEWS
		else if (strcmp(child->Value(),"perspective")==0)
		{
			string id=child->Attribute("id");
		
		
			if(	map_vistas.find(id)!=map_vistas.end()){
				cout<<"vista ja existe"<<endl;
				return -1;
			}
			else{
				float near;
				child->QueryFloatAttribute("near", &near);
				float far;
				child->QueryFloatAttribute("far", &far);
				float angle;
				child->QueryFloatAttribute("angle", &angle);
				TiXmlElement *Pchild=child->FirstChildElement();
				XYZ from;
				XYZ to;
				while(Pchild){
					if(strcmp(Pchild->Value(),"from")==0){
						float x;
						Pchild->QueryFloatAttribute("x", &x);
						float y;
						Pchild->QueryFloatAttribute("y", &y);
						float z;
						Pchild->QueryFloatAttribute("z", &z);
						XYZ from1(x,y,z);
						from=from1;
					}
					else if(strcmp(Pchild->Value(),"to")==0){
						float x;
						Pchild->QueryFloatAttribute("x", &x);
						float y;
						Pchild->QueryFloatAttribute("y", &y);
						float z;
						Pchild->QueryFloatAttribute("z", &z);
						XYZ to1(x,y,z);
						to=to1;
					}
					Pchild=Pchild->NextSiblingElement();
				}
			View *view1=new Prespective(angle,from,to);
			view1->setNear(near);
			view1->setFar(far);
			view1->setid(id);
			map_vistas.insert(pair<string,View*>(id,view1));
			}
		}
		else if (strcmp(child->Value(),"ortho")==0)
		{
			string id=child->Attribute("id");
		// left="ff" right="ff" top="ff" bottom="ff" />
			if(findId(map_vistas, id)){
				cout<< "ID repetido\n Não será adicionado"<<endl;
			}
			else{
			float near;
			child->QueryFloatAttribute("near", &near);
			float far;
			child->QueryFloatAttribute("far", &far);
			float left;
			child->QueryFloatAttribute("left", &left);
			float right;
			child->QueryFloatAttribute("right", &right);
			float top;
			child->QueryFloatAttribute("top", &top);
			float bot;
			child->QueryFloatAttribute("bottom", &bot);
				

			View *view1=new Ortho(left,right,top,bot);
			view1->setNear(near);
			view1->setFar(far);
			view1->setid(id);
			map_vistas.insert(pair<string,View*>(id,view1));
			}
		}
		//LUZES
		if (strcmp(child->Value(),"omni")==0)
		{
			string id=child->Attribute("id");
		
			if(	map_luzes.find(id)!=map_luzes.end()){
				cout<<"luz ja existe"<<endl;
				return -1;
			}
			else{
				bool enable=atoi(child->Attribute("enabled"));

				TiXmlElement *Pchild=child->FirstChildElement();
				XYZ x1;
				Omni *light1=new Omni(id);
				light1->setEnable(enable);
				while(Pchild){

					if(strcmp(Pchild->Value(),"location")==0){
						float x = atof(Pchild->Attribute("x"));
						float y = atof(Pchild->Attribute("y"));
						float z = atof(Pchild->Attribute("z"));
						
						if(Pchild->Attribute("w")!=NULL){
							float w = atof(Pchild->Attribute("w"));
							XYZ x2(x,y,z,w);
							x1=x2;
						}
						else{
							XYZ x2(x,y,z);
							x1=x2;
						}
						light1->setLocation(x1);
					}
					else if(strcmp(Pchild->Value(),"ambient")==0){
						float r = atof(Pchild->Attribute("r"));
						float g = atof(Pchild->Attribute("g"));
						float b = atof(Pchild->Attribute("b"));
						float a = atof(Pchild->Attribute("a"));
						RGBA r1(r,g,b,a);
						light1->setAmbient(r1);
					}
					else if(strcmp(Pchild->Value(),"diffuse")==0){
						float r = atof(Pchild->Attribute("r"));
						float g = atof(Pchild->Attribute("g"));
						float b = atof(Pchild->Attribute("b"));
						float a = atof(Pchild->Attribute("a"));
						RGBA r1(r,g,b,a);
						light1->setDiffuse(r1);
					}
					else if(strcmp(Pchild->Value(),"specular")==0){
						float r = atof(Pchild->Attribute("r"));
						float g = atof(Pchild->Attribute("g"));
						float b = atof(Pchild->Attribute("b"));
						float a = atof(Pchild->Attribute("a"));
						RGBA r1(r,g,b,a);
						light1->setSpecular(r1);
					}
				
						Pchild=Pchild->NextSiblingElement();
					
				}
					map_luzes.insert(pair<string,Light*>(id,light1));			
			}
		}
		else if (strcmp(child->Value(),"spot")==0){
			string id=child->Attribute("id");
		
			if(	map_luzes.find(id)!=map_luzes.end()){
				cout<<"luz ja existe"<<endl;
				return -1;
			}
			else{
				bool enable=atoi(child->Attribute("enabled"));
				float angle=atof(child->Attribute("angle"));
				float exp=atof(child->Attribute("exponent"));

				TiXmlElement *Pchild=child->FirstChildElement();
				XYZ x1;
				Spot *light1=new Spot(id);
				light1->setEnable(enable);
				light1->setAngle(angle);
				light1->setExponent(exp);

				while(Pchild){

					if(strcmp(Pchild->Value(),"location")==0){
						float x = atof(Pchild->Attribute("x"));
						float y = atof(Pchild->Attribute("y"));
						float z = atof(Pchild->Attribute("z"));
						
						if(Pchild->Attribute("w")!=NULL){
							float w = atof(Pchild->Attribute("w"));
							XYZ x2(x,y,z,w);
							x1=x2;
						}
						else{
							XYZ x2(x,y,z);
							x1=x2;
						}
						light1->setLocation(x1);
					}
					else if(strcmp(Pchild->Value(),"ambient")==0){
						float r = atof(Pchild->Attribute("r"));
						float g = atof(Pchild->Attribute("g"));
						float b = atof(Pchild->Attribute("b"));
						float a = atof(Pchild->Attribute("a"));
						RGBA r1(r,g,b,a);
						light1->setAmbient(r1);
					}
					else if(strcmp(Pchild->Value(),"diffuse")==0){
						float r = atof(Pchild->Attribute("r"));
						float g = atof(Pchild->Attribute("g"));
						float b = atof(Pchild->Attribute("b"));
						float a = atof(Pchild->Attribute("a"));
						RGBA r1(r,g,b,a);
						light1->setDiffuse(r1);
					}
					else if(strcmp(Pchild->Value(),"specular")==0){
						float r = atof(Pchild->Attribute("r"));
						float g = atof(Pchild->Attribute("g"));
						float b = atof(Pchild->Attribute("b"));
						float a = atof(Pchild->Attribute("a"));
						RGBA r1(r,g,b,a);
						light1->setSpecular(r1);
					}
					else if(strcmp(Pchild->Value(),"target")==0){
						float x = atof(Pchild->Attribute("x"));
						float y = atof(Pchild->Attribute("y"));
						float z = atof(Pchild->Attribute("z"));

						XYZ tar(x,y,z);
						light1->setTarget(tar);
					}
				
						Pchild=Pchild->NextSiblingElement();
					
				}
				map_luzes.insert(pair<string,Light*>(id,light1));
			
			}

		}
		
		//TEXTURAS
		else if (strcmp(child->Value(),"texture")==0)
		{
			string id=child->Attribute("id");
		
			if(	map_texturas.find(id)!=map_texturas.end()){
				cout<<"textura ja existe"<<endl;
				return -1;
			}
			else{
				string filename=child->Attribute("file");
				float ls=atof(child->Attribute("length_s"));
				float lt=atof(child->Attribute("length_t"));
				int counter=(int)map_texturas.size();
				Textura t1(counter,filename,ls,lt);
			map_texturas.insert ( pair<string,Textura>(id,t1) );
			char* fn;
			fn = new char [map_texturas[id].filename.size()+1];
			strcpy (fn, map_texturas[id].filename.c_str());
			if(	pixmap.readBMPFile(fn)!=1)
				cout<<"erro na leitura da textura"<<endl;
			int cn=map_texturas[id].counter+ntheme*100;
			pixmap.setTexture(cn);
	
			}
		}

		//MATERIAIS
		else if (strcmp(child->Value(),"material")==0)
		{
			string id=child->Attribute("id");
		if(	map_materiais.find(id)!=map_materiais.end()){
				cout<<"material ja existe"<<endl;
				return -1;
			}
			else{
					
					RGBA rg1;
					float*	em,*amb,*dif,*sp;
					float shine;
					TiXmlElement *Pchild=child->FirstChildElement();
					while(Pchild){

						//	if (strcmp(child->Value(),"transformation")==0)
						//if(strcmp(Pchild->Value(),"emission")){
						if(strcmp(Pchild->Value(),"emission")==0){
							float r=atof(Pchild->Attribute("r"));
							float g=atof(Pchild->Attribute("g"));
							float b=atof(Pchild->Attribute("b"));
							float a=atof(Pchild->Attribute("a"));
							float em1[4]={r,g,b,a};
							em=em1;
						}
						else if(strcmp(Pchild->Value(),"ambient")==0){
								float r=atof(Pchild->Attribute("r"));
							float g=atof(Pchild->Attribute("g"));
							float b=atof(Pchild->Attribute("b"));
							float a=atof(Pchild->Attribute("a"));
							float amb1[4]={r,g,b,a};
							amb=amb1;
						}
						else if(strcmp(Pchild->Value(),"diffuse")==0){
								float r=atof(Pchild->Attribute("r"));
							float g=atof(Pchild->Attribute("g"));
							float b=atof(Pchild->Attribute("b"));
							float a=atof(Pchild->Attribute("a"));
							float dif1[4]={r,g,b,a};
							dif=dif1;
						}
						else if(strcmp(Pchild->Value(),"specular")==0){
							float r=atof(Pchild->Attribute("r"));
							float g=atof(Pchild->Attribute("g"));
							float b=atof(Pchild->Attribute("b"));
							float a=atof(Pchild->Attribute("a"));
							float sp1[4]={r,g,b,a};
							sp=sp1;
						}
						else if(strcmp(Pchild->Value(),"shininess")==0){
							float shine1=atof(Pchild->Attribute("value"));
							shine=shine1;
						}
						Pchild=Pchild->NextSiblingElement();
					}
						 Material mat= Material(em,amb,dif,sp,shine);
						 map_materiais.insert ( pair<string,Material>(id,mat) );
			}
		}

		
		//----------------------------------------
		//TRANSFORMATIONS
		else if (strcmp(child->Value(),"transformation")==0)
		{
			
			float * m1;
			m1=(float*)malloc(16*sizeof(float*));
			m1[0]=0;

			string id=child->Attribute("id");
			if(	map_matrizes.find(id)!=map_matrizes.end()){
				cout<<"matriz ja existe"<<endl;
				return -1;
			}

			
			TiXmlElement *Pchild=child->FirstChildElement();
			glPushMatrix();
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			while (Pchild){
					if(strcmp(Pchild->Value(),"translate")==0){
						glTranslatef(atof(Pchild->Attribute("x")),atof(Pchild->Attribute("y")),atof(Pchild->Attribute("z")));
					}
					else if(strcmp(Pchild->Value(),"rotate")==0){
						if(strcmp(Pchild->Attribute("axis"),"x")==0)
							glRotatef(atof(Pchild->Attribute("angle")),1,0,0);
						else if(strcmp(Pchild->Attribute("axis"),"y")==0)
							glRotatef(atof(Pchild->Attribute("angle")),0,1,0);
						else if(strcmp(Pchild->Attribute("axis"),"z")==0)
							glRotatef(atof(Pchild->Attribute("angle")),0,0,1);
					}
					else if(strcmp(Pchild->Value(),"scale")==0) glScalef(atof(Pchild->Attribute("x")),atof(Pchild->Attribute("y")),atof(Pchild->Attribute("z")));
					Pchild=Pchild->NextSiblingElement();
			}
			glGetFloatv(GL_MODELVIEW_MATRIX, &m1[0]);
			//cout<<m1[0]<<" "<<m1[1]<<" "<<m1[2]<<" "<<m1[3]<<endl;
			//cout<<m1[4]<<" "<<m1[5]<<" "<<m1[6]<<" "<<m1[7]<<endl;
			//cout<<m1[8]<<" "<<m1[9]<<" "<<m1[10]<<" "<<m1[11]<<endl;
			//cout<<m1[12]<<" "<<m1[13]<<" "<<m1[14]<<" "<<m1[15]<<endl;
			map_matrizes.insert ( pair<string,float*>(id, m1) );
			glPopMatrix();
			
		}


		//----------------------------------------
		//PRIMITIVES
		if (strcmp(child->Value(),"primitive")==0)
		{
			TiXmlElement *Pchild=child->FirstChildElement();
			string val=Pchild->Value();
			string material="";
			string texture="";
			string id=child->Attribute("id");
			//procura material
			if(	map_primitivas.find(id)!=map_primitivas.end()){
				cout<<"primitiva ja existe"<<endl;
				return -1;
			}
			Pchild=child->FirstChildElement("material");
			if(Pchild!=NULL){
				material = Pchild->Attribute("id");
			}
			else{	
					cout<<"Nao existe material na primitiva de id: "<<id<<endl;
					return -1;
			}

			//procura textura
			Pchild=child->FirstChildElement("texture");
			if(Pchild!=NULL){
				texture = Pchild->Attribute("id");
			}
			else{	
					cout<<"Nao existe textura na primitiva de id: "<<id<<endl;
					return -1;
			}

			//procura forma geometrica
			bool found=0;
			
			Pchild=child->FirstChildElement("rectangle");
			if(Pchild!=NULL){	
				Rectangulo *r = new Rectangulo(atof(Pchild->Attribute("x1")),atof(Pchild->Attribute("x2")),atof(Pchild->Attribute("y1")),atof(Pchild->Attribute("y2")));
				r->id="rectangle";
				r->setMaterial(material);
				r->setTexture(texture);
				found=true;
				map_primitivas.insert ( pair<string,No*>(id, r ) );
			}
			else if((Pchild = child->FirstChildElement("triangle")) != NULL&&!found) {
				Triangulo *t = new Triangulo(atof(Pchild->Attribute("x1")),atof(Pchild->Attribute("x2")),atof(Pchild->Attribute("x3")),atof(Pchild->Attribute("y1")),atof(Pchild->Attribute("y2")),atof(Pchild->Attribute("y3")),atof(Pchild->Attribute("z1")),atof(Pchild->Attribute("z2")),atof(Pchild->Attribute("z3")));
				t->id="triangle";
				t->setMaterial(material);
				t->setTexture(texture);
				found=true;
				map_primitivas.insert ( pair<string,No*>(id, t ) );
			}
			 Pchild=child->FirstChildElement("cylinder");
			if(Pchild!=NULL&&!found){
				Cilindro *c = new Cilindro(atof(Pchild->Attribute("base")),atof(Pchild->Attribute("top")),atof(Pchild->Attribute("height")),atof(Pchild->Attribute("slices")),atof(Pchild->Attribute("stacks")));
				c->id="cylinder";
				c->setMaterial(material);
				c->setTexture(texture);
				found=true;
				map_primitivas.insert ( pair<string,No*>(id, c ) );
			}
			Pchild=child->FirstChildElement("sphere");
			if(Pchild!=NULL&&!found){
				Esfera *s = new Esfera(atof(Pchild->Attribute("radius")),atof(Pchild->Attribute("slices")),atof(Pchild->Attribute("stacks")));
				s->id="sphere";
				s->setMaterial(material);
				s->setTexture(texture);
				found=true;
				map_primitivas.insert ( pair<string,No*>(id, s ) );
			}
			 Pchild=child->FirstChildElement("torus");
			if(Pchild!=NULL&&!found){
				Torus *t  = new Torus(atof(Pchild->Attribute("inner")),atof(Pchild->Attribute("outer")),atof(Pchild->Attribute("slices")),atof(Pchild->Attribute("loops")));
				t->id="torus";
				t->setMaterial(material);
				t->setTexture(texture);
				found=true;
				map_primitivas.insert ( pair<string,No*>(id, t ) );
			}

			if(!found){
				cout<<"Nao encontrei forma geometrica na primitiva de id: "<<id<<endl;
			}

			// é um nó
			// acede aos dados do nó e aplica materiais, texturas, transformações
			// para aceder ao nó, existiriam várias alternativas
			// aqui implementa-se uma pesquisa básica
		//	TiXmlElement *node=findChildByAttribute(nodesElement,"id",child->Attribute("id"));
		/*	if(node==NULL)
				cout<<"mas node é nulo"<<endl;
			if (node)
			{
				printf("%s     - Material id: '%s' \n", prefix, node->FirstChildElement("material")->Attribute("id"));
				printf("%s     - Texture id: '%s' \n", prefix, node->FirstChildElement("texture")->Attribute("id"));

				// repetir para outros detalhes do nó
			}
			*/

			// processa recursivamente os seus descendentes
		//	child=child->NextSiblingElement();
		}
		//----------------------------------------
		else if (strcmp(child->Value(),"component")==0)
		{
			TiXmlElement *Pchild=child->FirstChildElement();
			string val=Pchild->Value();
			string transformation="";
			bool ref=true;
			bool haskey=false;
			float * m1;
			m1=(float*)malloc(16*sizeof(float*));
			m1[0]=0;
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			glGetFloatv(GL_MODELVIEW_MATRIX, m1);
			char kb_key='\0';
			string material="";
			vector<string> materials;
			string texture="";
			string id=child->Attribute("id");
			Pchild=child->FirstChildElement("transformation");
			if(Pchild!=NULL){
			
				TiXmlElement *P2child=Pchild->FirstChildElement();
		
				while(P2child){
			
					if(strcmp(P2child->Value(),"transformationref")==0){
						transformation=P2child->Attribute("id");
						
						P2child=P2child->NextSiblingElement();
					}
			
					else if((strcmp(P2child->Value(),"translate")==0)||
						(strcmp(P2child->Value(),"rotate")==0)||
						(strcmp(P2child->Value(),"scale")==0))
					{	
				
						ref=false;
						glPushMatrix();
						glMatrixMode( GL_MODELVIEW );
						glLoadIdentity();
						while (P2child)
						{
							if(strcmp(P2child->Value(),"translate")==0)
							{
								glTranslatef(atof(P2child->Attribute("x")),atof(P2child->Attribute("y")),atof(P2child->Attribute("z")));
							}
							else if(strcmp(P2child->Value(),"rotate")==0)
							{
								if(strcmp(P2child->Attribute("axis"),"x")==0)
									glRotatef(atof(P2child->Attribute("angle")),1,0,0);
								else if(strcmp(P2child->Attribute("axis"),"y")==0)
									glRotatef(atof(P2child->Attribute("angle")),0,1,0);
								else if(strcmp(P2child->Attribute("axis"),"z")==0)
									glRotatef(atof(P2child->Attribute("angle")),0,0,1);
							}
							else if(strcmp(P2child->Value(),"scale")==0){
								glScalef(atof(P2child->Attribute("x")),atof(P2child->Attribute("y")),atof(P2child->Attribute("z")));
							}
							P2child=P2child->NextSiblingElement();
						}
						glGetFloatv(GL_MODELVIEW_MATRIX, &m1[0]);
						glPopMatrix();

					}
					else{	
							
						cout<<"Nao existe transformaçoes na componente de id: "<<id<<endl;
						return -1;
					}
				}
				Pchild=Pchild->NextSiblingElement();
			}
			else{	
					cout<<"Nao existe transformaçoes na componente de id: "<<id<<endl;
					return -1;
			}
			//procura material
			Pchild=child->FirstChildElement("materials");
			if(strcmp(Pchild->Value(),"materials")==0)
			{
				TiXmlAttribute* key=Pchild->FirstAttribute();
				if(key!=NULL)
				{//associa botao do keyboard
					haskey=true;
					kb_key = key->Value()[0];
					Pchild=Pchild->FirstChildElement("material");
					while (Pchild)
					{
					material=Pchild->Attribute("id");
					materials.push_back(material);
					Pchild=Pchild->NextSiblingElement();
					}
				}
				else{//caso contrario pega no material
					Pchild=Pchild->FirstChildElement("material");
					material=Pchild->Attribute("id"); 
				}
			}
			else{	
					cout<<"Nao existe materiais na componente de id: "<<id<<endl;
					return -1;
			}

			//procura textura
			Pchild=child->FirstChildElement("texture");
			if(Pchild!=NULL){
				texture = Pchild->Attribute("id");
			}
			else{	
					cout<<"Nao existe textura na componente de id: "<<id<<endl;
					return -1;
			}

			Pchild=child->FirstChildElement("children");
			if(Pchild!=NULL){
				Pchild=Pchild->FirstChildElement();
				vector<string>comp;
				vector<string>prim;
					while (Pchild)
					{
					if(strcmp(Pchild->Value(),"primitiveref")==0)
						{
							prim.push_back(Pchild->Attribute("id"));
						}
						else if(strcmp(Pchild->Value(),"componentref")==0)
						{
							comp.push_back(Pchild->Attribute("id"));
						}
					Pchild=Pchild->NextSiblingElement();
					}
					//cout<<"ref is "<<ref<<endl;
					Component *c= new Component(m1,prim,comp,transformation,ref);
					c->id=id;
					if(haskey)
					{
							c->setMaterialvector(materials);
							material=materials[0];
					}
					c->setKey(kb_key);
					c->setMaterial(material);
					c->setTexture(texture);
					//c->transformacoes=;
					//cout<<id<<" inserted in components map"<<endl;
					map_componentes.insert ( pair<string,No*>(id, c ) );
			}
			else{	
					cout<<"Nao existe children na componente de id: "<<id<<endl;
					return -1;
			}

		}
		child=child->NextSiblingElement();
	}
	return 0;

}

int SceneLoader::loadScene2(const char * fileName)
{

	// Read string from file

	TiXmlDocument doc( fileName );
	bool loadOkay = doc.LoadFile();
	if ( !loadOkay )
	{
		printf( "Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc() );
		exit( 1 );
	}

	
	//viewsElement =  root->FirstChildElement( "views" );
	//default_View=viewsElement->Attribute("default");
	ilumElement =  root->FirstChildElement( "illumination" );

	//graphElement =  root->FirstChildElement( "Graph" );

	//NOVOS
	rootElement = root->FirstChildElement( "scene" );
	textsElement =  root->FirstChildElement( "textures" );
	matsElement = root->FirstChildElement( "materials" );
	primElement = root->FirstChildElement( "primitives" );
	compElement = root->FirstChildElement( "components" );
	transElement =  root->FirstChildElement( "transformations" );
	lightsElement =  root->FirstChildElement( "lights" );


	//cout<<"Init"<<endl;
	// Inicialização
	// Um exemplo de um conjunto de nós bem conhecidos e obrigatórios

	if (rootElement == NULL){
		printf("Bloco scene não encontrado\n");
		cout<<"Nao existe root element,projecto abortado"<<endl;
		string dummy;
		cin>>dummy;
		return -1;
	}else
	{
		if (strcmp(rootElement->Value(),"scene")==0){
		root_id=rootElement->Attribute("root");
		if(root_id==""){
				cout<<"Nao existe root element,projecto abortado"<<endl;
			
			string dummy;
			cin>>dummy;
			return -1;
		}
		scene_length_axis=atof(rootElement->Attribute("axis_length"));

	}
	}
	// Validação dos outros grupos seria feita aqui


	// render graph
	// iteração recursiva
	//if(processGraphNode(viewsElement,0)==-1||map_vistas.size()==0)
		//return -1;

	if(processGraphNode(ilumElement,0)==-1)
		return -1;
	
	if(processGraphNode(lightsElement,0)==-1||map_luzes.size()==0)
		return -1;
	
	if(processGraphNode(textsElement,0)==-1)
		return -1;
	
	if(processGraphNode(matsElement,0)==-1)
		return -1;

	if(processGraphNode(transElement,0)==-1)
		return -1;

	if(processGraphNode(primElement,0)==-1)
		return -1;
	
	if(processGraphNode(compElement,0)==-1)
		return -1;
	return 0;
}