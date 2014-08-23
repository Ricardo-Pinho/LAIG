// aulas.cpp : Defines the entry point for the console application.
//
#pragma once
#include <GL/glui.h>

#include "Parser/SceneLoader.h"



#include <math.h>
#include <sstream>
#include <time.h>
#include <math.h>
#include "pick.h"
#include <vector>
#include <string>
#include <iostream>
#include <time.h>

using namespace std;
#define DIMX 500
#define DIMY 500
#define INITIALPOS_X 100
#define INITIALPOS_Y 100

#define BUFSIZE 512

#define VLENGTH 3
#define mesaList 1

extern bool executeOp(int opcao);
/*
1=socketConect
-1= quit
*/
extern string recebe(char *ans);
extern void envia(char *s, int len);
void movePeca();
void turn(int dir,double line,int keepDest);

//extern bool socketConnect();
//sockets ops


string toString2(char *ans){
	string str="";
	int i=0;
	while(ans[i]!=NULL){
		str+=ans[i];
		i++;
	}
	return str;
}
vector<vector<int> > toVecVector2(string answer){
	int i=0;
	int numberOfPar=0;
	vector<vector<int> > result;

	if(answer[i]=='['){
		i++;
		numberOfPar++;
	}
	else
		return result;

	vector<int> line;
	while(i<answer.size()){
		if(numberOfPar==0)
			break;
		if(answer[i]=='['){
			numberOfPar++;
		}
		else if(answer[i]==']'){
			if(numberOfPar>1) {
			result.push_back(line);
			vector<int> clean;
			line=clean;
			}
			numberOfPar--;
			
		}
		else if(answer[i]==','){
		}
		else{
			int num = answer[i]-48; 
			line.push_back(num);
		}
		i++;
	}
	return result;
}
string toList2(vector<vector<int> > _tab){
	string list="[";
	int j=0,k=0;
	cout<<"List:"<<endl;
	for(j=0;j<_tab.size();j++){
		list+="[";
		for(k=0;k<_tab[j].size();k++){
			char n=_tab[j][k]+48;
			list+=n;
			if(k!=_tab[j].size()-1)
				list+=",";
		}
		if(j!=_tab.size()-1)
			list+="],";
		else
			list+="]";

	}
	list+="]";
	return list;
}


//----------------------------------------------------//
float xy_aspect;		// aspect ratio da area de visualizacao
int window_w=DIMX;
int window_h=DIMY;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

int light_number=2;
int camera_step=0;
// variaveis globais
int main_window;
GLUI *glui, *glui2, *glui3, *glui4;

float light0_kc=0.0;
float light0_kl=0.0;
float light0_kq=0.1;
double light0x = 0;
double light0y = 0;
double light0z = -1;

// declarações para as stripes que formam o plano
double i,j;
double di, limi=DIMX, divisoes_i = 60;	//60
double dj, limj=DIMY, divisoes_j = 100;	//100


float mat1_shininess[] = {20.0}; 
float mat1_specular[] = {0.3, 0.3, 0.3, 1.0}; /* specular reflection. */
float mat1_diffuse[] = {0.3, 0.3, 0.3, 1.0}; /* diffuse reflection. */

RGBpixmap pix[20];
GLUquadric* glQ;

SceneLoader *scene;
SceneLoader *scene1;
SceneLoader *scene2;
SceneLoader *scene3;
int wireframe =false;
int gameStart=false;
int createStart=0;
int temas=0;
int oldtheme=0;
int timec=0;
int ccamera=0;
time_t start,tend;

int Gd;

GLUI_EditText *clockt;
GLUI_Panel *c_panel;
GLUI_Checkbox *check;
GLUI_Checkbox *check1;



// variaveis usadas para o picking
float dx=0.0, dy=0.0, dz=0.0, dxAc=0.0, dzAc=0.0;	// variaveis auxiliares de posicao
float Xini=0, Yini=0;
picking *pk;										// apontador para a classe que controla a posicao dos objectos
//manter dados da peca selecionada
bool selected=false;
int turning=-1;
double posX=-1;
double posZ=-1;
bool onTheMove=false;//se estiver true a peça esta-se a mover lg  nao é possivel jogar
const int mili_secs = 30;//50
const float cmili_secs = 0.05;



//Variaveis Tabuleiro
double col_linhas=7.0;
double dimensao=10.0;
double dimensaoY=1.0;
vector< vector<int> > tab;
//Variaveis das peças estaticas
double jogador=1;// 1 ou 2 - para saber quem está a jogar 
double radiusPeca=0.6;
bool fase1=true;
bool fase2=false;
bool fase3=false;
bool faseCancel=false;
//Jogador 1
double iniPeca1X=3.5;
double iniPeca1Y=0.0;
double iniPeca1Z=-9.0;
double peca1X=iniPeca1X;
double peca1Y=iniPeca1Y;
double peca1Z=iniPeca1Z;
double movePeca1X=0;
double movePeca1Y=0;
double movePeca1Z=0;
//Jogador 2
double iniPeca2X=-iniPeca1X;
double iniPeca2Y=iniPeca1Y;
double iniPeca2Z=iniPeca1Z;
double peca2X=iniPeca2X;
double peca2Y=iniPeca2Y;
double peca2Z=iniPeca2Z;
double movePeca2X=0;
double movePeca2Y=0;
double movePeca2Z=0;


//scores
int scoreJog1=0;
int scoreJog2=0;
int win=-1;//novo jogo quando win!=-1
int AIlevel=0;//0; esta a 1 so para testes
int doubleAi=0;
int jogadorAI=2;//por default, mas num jogo PC vs PC varia entre 1 e 2
bool vezAI=false;

//Undo e movie
#define INSERIR "inserir"
#define RODAR "rodar"
vector<pair<string, vector<int> > >  jogadas;
bool replay=false;
int current_move=0;
/*

1ªstring="inserir" ou rodar"
vector<int>=
	int[0]=jogador;
    se "inserir" = int[1]=Y e int[2]=X  <=> posicao(Y;X) 
    se "rodar" = int[1]=Direcao e int[2]=Linha/Coluna int[3]=PecaDestruida
rodar: int é 1-rodar para cima, 2-baixo, 3-esquerda,4-direita
*/

int destroyed=0;//peça retirada


void replayAll(int playtoreplay);
void nextReplay(int dummy);
void display(void);

void processIllumination(){
		// Define que modelo de iluminacao utilizar; consultar o manual de referencia
	 //cout<<scene->illumination.getAmbient()[0]<<scene->illumination.getAmbient()[1]<<scene->illumination.getAmbient()[2]<<endl;
	 float *amb=scene->illumination.getAmbient();
	  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);  // define luz ambiente
	  if(scene->illumination.doublesided)
		    glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	  else
		glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	    if(scene->illumination.local) glLightModelf (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		else glLightModelf (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glClearColor(scene->illumination.getBackground()[0], scene->illumination.getBackground()[1],scene->illumination.getBackground()[2],scene->illumination.getBackground()[3]);

 }

 void processLights(){
	 
	 map<string, Light*>::iterator it;
	 for(it= scene->map_luzes.begin();it!=scene->map_luzes.end();it++){
		 if(it->second->enabled){
			 it->second->makeLight(light_number);
			 light_number++;
		 }
		 else{
			 glDisable(GL_LIGHT0+light_number);
			 light_number++;

		 }
		 
	 }
	 light_number=0;

 }

/* Function to normalise a vector to unit length */
void normalise(GLdouble *vec)
{
  GLdouble length = 0.0;
  int i;

  for (i=0;i<VLENGTH;i++){
     length += vec[i]*vec[i]; 
  }
  length= (GLdouble) sqrt((double)length); 

  for (i=0;i<VLENGTH;i++){
     vec[i] = vec[i]/length; 
  }
}


void updateCamera1(int dummy)
{
	if(camera_step==0&&(view_rotate[0]<1||view_rotate[2]>0)){
	view_rotate[0]=view_rotate[0]+cmili_secs;
	view_rotate[2]=view_rotate[2]-cmili_secs;
	view_rotate[8]=view_rotate[8]+cmili_secs;
	view_rotate[10]=view_rotate[10]+cmili_secs;
	if(view_rotate[0]>=1) camera_step=1;
	glutTimerFunc(mili_secs, updateCamera1, 0);
	}

	else if(camera_step==1&&(view_rotate[0]>0||view_rotate[2]>-1)){
	view_rotate[0]=view_rotate[0]-cmili_secs;
	view_rotate[2]=view_rotate[2]-cmili_secs;
	view_rotate[8]=view_rotate[8]+cmili_secs;
	view_rotate[10]=view_rotate[10]-cmili_secs;
	if(view_rotate[0]<=0){
	glui2->enable();
	camera_step=2;
	}
	else glutTimerFunc(mili_secs, updateCamera1, 0);
	}
	

}

void updateCamera2(int dummy)
{
	if(camera_step==0&&(view_rotate[0]<1||view_rotate[2]<0)){
	view_rotate[0]=view_rotate[0]+cmili_secs;
	view_rotate[2]=view_rotate[2]+cmili_secs;
	view_rotate[8]=view_rotate[8]-cmili_secs;
	view_rotate[10]=view_rotate[10]+cmili_secs;
	if(view_rotate[0]>=1) camera_step=1;
	glutTimerFunc(mili_secs, updateCamera2, 0);
	}

	else if(camera_step==1&&(view_rotate[0]>0||view_rotate[2]<1)){
	view_rotate[0]=view_rotate[0]-cmili_secs;
	view_rotate[2]=view_rotate[2]+cmili_secs;
	view_rotate[8]=view_rotate[8]-cmili_secs;
	view_rotate[10]=view_rotate[10]-cmili_secs;
	if(view_rotate[0]<=0){
	glui2->enable();
	camera_step=2;
	}
	else glutTimerFunc(mili_secs, updateCamera2, 0);
	}
}

void newellSquare(GLdouble *vec1,GLdouble *vec2,GLdouble *vec3,GLdouble *vec4,GLdouble *normal)
{
  normal[0] = (vec1[1]-vec2[1])*(vec1[2]+vec2[2]) + 
	      (vec2[1]-vec3[1])*(vec2[2]+vec3[2]) + 
	      (vec3[1]-vec4[1])*(vec3[2]+vec4[2]) +
	      (vec4[1]-vec1[1])*(vec4[2]+vec1[2]);
  normal[1] = (vec1[2]-vec2[2])*(vec1[0]+vec2[0]) + 
	      (vec2[2]-vec3[2])*(vec2[0]+vec3[0]) + 
	      (vec3[2]-vec4[2])*(vec3[0]+vec4[0]) +
	      (vec4[2]-vec1[2])*(vec4[0]+vec1[0]);
  normal[2] = (vec1[0]-vec2[0])*(vec1[1]+vec2[1]) +		
	      (vec2[0]-vec3[0])*(vec2[1]+vec3[1]) + 
	      (vec3[0]-vec4[0])*(vec3[1]+vec4[1]) +
	      (vec4[0]-vec1[0])*(vec4[1]+vec1[1]);

  normalise(normal);

}
void clear_vector(){//pode vir daqui o erro ao limpar
	vector< vector<int> > tab2;
	vector<int> inner (col_linhas,0);
	int i=0;
	for(i=0;i<col_linhas;i++){
		tab2.push_back(inner);
	}
	tab=tab2;
}
void peca(GLdouble radius,int player){//variavel player para mudar a textura consoante o jogador
	
		double height=0.25;
		double slices=20;
		double stacks=8;
		glEnable(GL_COLOR_MATERIAL);
		if(player==1)
		{	
			float mat1_emission[] = {1.0, 0.0, 0.0, 0.0};
			float mat1_specular[] = {0.4, 0.0, 0.0, 1.0};	/* specular reflection. */
			float mat1_diffuse[] =  {0.4, 0.0, 0.0, 1.0};	/* diffuse reflection. */
			float mat1_ambient[] =  {0.4, 0.0, 0.0, 1.0};	/* ambient reflection. */
			float mat1_shininess[] =  {20.0};

			//cout<<id<<" is giving value "<<material.second.emr<<endl;//" "<<material.second.difg<<" "<<material.second.difb<<" "<<material.second.difa<<endl;

			glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
			glMaterialfv(GL_FRONT, GL_EMISSION,  mat1_emission);
			glMaterialfv(GL_FRONT, GL_SPECULAR,  mat1_specular);
			glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat1_diffuse);
			glMaterialfv(GL_FRONT, GL_AMBIENT,   mat1_ambient);
		}
		else
		{
				float mat1_emission[] = {0.0, 0.0, 0.4, 0.0};
				float mat1_specular[] = {0.0, 0.0, 0.4, 1.0};	/* specular reflection. */
				float mat1_diffuse[] =  {0.0, 0.0, 0.4, 1.0};	/* diffuse reflection. */
				float mat1_ambient[] =  {0.0, 0.0, 0.4, 1.0};	/* ambient reflection. */
				float mat1_shininess[] =  {20.0};

				//cout<<id<<" is giving value "<<material.second.emr<<endl;//" "<<material.second.difg<<" "<<material.second.difb<<" "<<material.second.difa<<endl;

				glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION,  mat1_emission);
				glMaterialfv(GL_FRONT, GL_SPECULAR,  mat1_specular);
				glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat1_diffuse);
				glMaterialfv(GL_FRONT, GL_AMBIENT,   mat1_ambient);
		}
		glPushMatrix();
	
		glRotatef(90,1,0,0);
	
		glPushMatrix();
		gluQuadricDrawStyle(glQ, GLU_FILL);
		gluQuadricTexture(glQ, 1);
		gluQuadricNormals(glQ, GLU_SMOOTH);
		gluCylinder(glQ, radius, radius, height, slices, stacks);
		glPopMatrix();

		glPushMatrix();
		glRotatef(180,0,1,0);
		gluDisk(glQ,0,radius,slices,1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0,0,height);
		gluDisk(glQ,0,radius,slices,1);
		glPopMatrix();

		glPopMatrix();
		glDisable(GL_COLOR_MATERIAL);
}
void draw_pecas(){
	int i=0,j=0;
	double dim=dimensao/2;
	double size=dimensao/col_linhas;
	
	for(i=0;i<tab.size();i++){
		
		for(j=0;j<tab[i].size();j++){
			
			if(tab[i][j]!=0){
				glPushName(7);//codigo de peça colocada 
				glPushName(j); 
				glPushName(i); 
				
				
				
				double x=j*size+size/2;
				double z=i*size+size/2;
				glPushMatrix();
				glTranslatef(x,0,z);
				if(tab[i][j]==1)
					peca(radiusPeca,1);
				else
					peca(radiusPeca,2);
				glPopMatrix();
				
				glPopName();
				glPopName(); 
				glPopName();
			}
			
		}
		
	}
	


}
void squares(GLdouble dimx, GLdouble dimy, GLdouble dimz){
	GLdouble dx=dimx/2.0, dy=dimy/2.0, dz=dimz/2.0;
	
	GLdouble sizeX=dimx/col_linhas;
	GLdouble sizeZ=dimz/col_linhas;

	GLdouble initialx=-dx;
	GLdouble initialz=-dz;
	double i=0.0,j=0.0;
	GLdouble thisx,thisz;
	GLdouble nextx,nextz;
	
	thisx=initialx;
	nextx=initialx;
	//espaço aconselhado
	//GLdouble space=sizeX/10.0;
	GLdouble space=0.5;

	for(;i<col_linhas;i++){
		//cout<<"thisX"<<thisx<<endl;
		//cout<<"nextX"<<nextx<<endl;
		glPushName(i); 
		thisx=nextx;
		nextx=thisx+sizeX;
		thisx+=space;
		nextx-=space;

		nextz=initialz;
		for(j=0;j<col_linhas;j++){
			glPushName(j); 
			//cout<<"thisZ"<<thisz<<endl;
			//cout<<"nextZ"<<nextz<<endl;
			thisz=nextz;
			nextz=thisz+sizeZ;	
			thisz+=space;
			nextz-=space;
			glBegin(GL_POLYGON);
				glVertex3d(thisx, dy, thisz);
				glVertex3d(nextx, dy, thisz);
				glVertex3d(nextx, dy, nextz);
				glVertex3d(thisx, dy, nextz);
			glEnd();
			
			glPopName(); 
			nextz+=space;
		}
		glPopName(); 

		nextx+=space;

	}
	glDisable(GL_TEXTURE_2D);

}
bool verificarFimJogo(){
	

	//envia comando para socket
		char enviado[254];
		stringstream mensagem;
		mensagem<<"fim_jogo("<<toList2(tab)<<").\n";
		strcpy(enviado,(mensagem.str()).c_str());
		
		envia(enviado,strlen(enviado));
		char recebido[254];
		string resp;
		resp=recebe(recebido);
		//examina resposta
		cout<<"resp is:"<<resp<<"???"<<endl;
		if(resp[0]=='1'&&win==-1){
			cout<<"ganhou jogador 1"<<endl;
			scoreJog1++;
			win=1;
			if(AIlevel>0){
			AIlevel=AIlevel-1;
			doubleAi=doubleAi+1;
			}
			cout<<AIlevel<<" "<<doubleAi<<endl;
			glui3->show();
			timec=0;
			return true;
		}
		else if(resp[0]=='2'&&win==-1){
			cout<<"ganhou jogador 2"<<endl;
			scoreJog2++;
			win=2;
			if(AIlevel>0){
			AIlevel=AIlevel-1;
			doubleAi=doubleAi+1;
			}
			cout<<AIlevel<<" "<<doubleAi<<endl;
			glui3->show();
			timec=0;
			return true;
		}
		else if(resp[0]=='3'&&win==-1){
			cout<<"empate"<<endl;
			scoreJog1++;
			scoreJog2++;
			win=3;
			if(AIlevel>0){
			AIlevel=AIlevel-1;
			doubleAi=doubleAi+1;
			}
			cout<<AIlevel<<" "<<doubleAi<<endl;
			glui3->show();
			timec=0;
			return true;
		}
		
		
		
	//Fim....

}
bool AIplay();
void goAIplay(int dummy){
	AIplay();
}

bool AIplay(){
		
		char enviado2[254];
		stringstream mensagem2;
		mensagem2<<"calcula_jogada("<<toList2(tab)<<","<<AIlevel<<","<<
		jogadorAI<<").\n";
		strcpy(enviado2,(mensagem2.str()).c_str());
		envia(enviado2,strlen(enviado2));
		char recebido2[254];
		string resp2;
		resp2=recebe(recebido2);
		//examina resposta
		vector<vector<int> >new_tab ;
		
		cout<<"\n\n\n---------------------------------------------------------------"<<endl;
		cout<<resp2<<endl;
		cout<<"---------------------------------------------------------------"<<endl;

		if(resp2.size()>6){
			string sub= resp2.substr (0,5);
			if(sub=="rodar"){
				if(doubleAi==1){
				cout<<"vai rodar"<<endl;

				string s_dir=resp2.substr (9,1);
				string s_line=resp2.substr (7,1);
				int i_dir=atoi(s_dir.c_str());
				int i_line=atoi(s_line.c_str());

				if(resp2[5]=='X'){
					if(i_dir==1)
						turn(2,(i_line-1),1);
					else if(i_dir==2)
						turn(4,(i_line-1),1);
				}
				else if(resp2[5]=='Y'){
					if(i_dir==1)
						turn(3,(i_line-1),1);
					else if(i_dir==2)
						turn(1,(i_line-1),1);
					
				}
				cout<<"rodou"<<endl;
				if(jogador==2){
					jogador=1;
				}
				else{
					jogador=2;
				}
				vezAI=false;
				posX=-1;
				posZ=-1;
				}
			}
			else{
				cout<<"SUB WAS:"<<sub<<endl;
				sub= resp2.substr (0,7);
				if(sub=="inserir"){
					string s_posZ=resp2.substr (10,1);
					string s_posX=resp2.substr (8,1);
					cout<<"AS STRING: in inserir it was : z:"<<s_posZ<<" and x:" <<s_posX<<endl;
					if(AIlevel!=1){
					posX=atoi(s_posX.c_str());
					posZ=atoi(s_posZ.c_str());
					}
					else{
					posX=atoi(s_posX.c_str())-1;
					posZ=atoi(s_posZ.c_str())-1;
					}
					cout<<"AS int: in inserir it was : z:"<<s_posZ<<" and x:" <<s_posX<<endl;
					fase1=true;
					vezAI=true;
					movePeca();
				}
				else{ 
					cout<<"SUB WAS: ELSE"<<sub<<endl;
					jogador=1;vezAI=false;
					posX=-1;
					posZ=-1;
				}
			}
		
			display();
		}
		else{
			cout << "This was printed:"<< resp2<<endl;
		}
		return true;
		
	}
void inserirPeca(double posZ, double posX, int _jogador){
	cout<<"INSERI::::::::::::::::::::::::::::::::::::::"<<endl;
	int i=0,j=0;
	tab[posZ][posX]= _jogador;
	//guardar jogada no vector jogada
	vector<int> ints;
	ints.push_back(jogador);
	ints.push_back(posZ);
	ints.push_back(posX);
	if(!replay){
		string operacao=INSERIR;
		pair <string ,vector<int>> play;
		play = make_pair (operacao,ints);

		jogadas.push_back(play);

		verificarFimJogo();
		
		if(jogador==1&&win==-1){
			if(ccamera==1){
			camera_step=0;
			glui2->disable();
			view_rotate[0]=0;view_rotate[1]=0;view_rotate[2]=1;view_rotate[3]=0;
			view_rotate[4]=0;view_rotate[5]=1;view_rotate[6]=0;view_rotate[7]=0;
			view_rotate[8]=-1;view_rotate[9]=0;view_rotate[10]=0;view_rotate[11]=0;
			view_rotate[12]=0;view_rotate[13]=0;view_rotate[14]=0;view_rotate[15]=1;
			glutTimerFunc(mili_secs,updateCamera1,0);
			}
			if(AIlevel==0)
				jogador=2;
		}else if(win==-1){
			if(ccamera==1){
			camera_step=0;
			glui2->disable();
			view_rotate[0]=0;view_rotate[1]=0;view_rotate[2]=-1;view_rotate[3]=0;
			view_rotate[4]=0;view_rotate[5]=1;view_rotate[6]=0;view_rotate[7]=0;
			view_rotate[8]=1;view_rotate[9]=0;view_rotate[10]=0;view_rotate[11]=0;
			view_rotate[12]=0;view_rotate[13]=0;view_rotate[14]=0;view_rotate[15]=1;
			glutTimerFunc(mili_secs,updateCamera2,0);
			}
			if(AIlevel==0)
				jogador=1;
		}
		
	}
	

	//calcula_jogada(Tab,Nivel,Jogador)
	//insere_pecaY(Y,X,Jogador,Tab)
	
	
	//envia comando para socket
	
	//}
	//Fim....

}
void turn(int dir,double line,int keepDest){//roda a linha dada na direcao dada. 1-rodar para cima, 2-baixo, 3-esquerda,4-direita. keepDest-se for 1 o jogo grava a peça destruida,senão ignora-a
	int i=0;
	
	if(dir==1){
		if(!replay){
			cout<<"Para cima"<<endl;
			if(keepDest==1)
				destroyed=tab[0][line];
			cout<<"Dest:"<<destroyed<<endl;
		
			//envia comando para socket
			char enviado[254];
			stringstream mensagem;
			mensagem<<"rodaX("<<(line+1)<<","<<
				toList2(tab)<<","<<0<<","<<
				2<<").\n";
			printf("MESSAGE IS:%s\n",(mensagem.str()).c_str());
			strcpy(enviado,(mensagem.str()).c_str());
			envia(enviado,strlen(enviado));
			char recebido[254];
			string resp;
			resp=recebe(recebido);
			cout<<"THIS IS RETURNED:\n-"<<resp<<endl;
			//examina resposta
			vector<vector<int> >new_tab ;
			if(resp.size()>3){
				new_tab = toVecVector2(resp);
				tab=new_tab;
			string answer2=toList2(new_tab);
			cout << "Valid command.prolog  answered: "/* << resp */<< endl;
			}
			else cout << "Incorrect command.prolog answered: " /*<< resp */<< endl;
		
		//Fim....
		}
		else{
			int i=0;
			int j=0;
			for(i=0;i <(tab.size()-1);i++){
				for(j=0;j<tab[i].size();j++) {
					if(j==line){
						tab[i][j]=tab[i+1][j];
					}
				}
			}
			tab[i][line]=0;
		}
	}
	else if(dir==2){

		cout<<"Para baixo"<<endl;
		if(!replay){
			if(keepDest==1)
				destroyed=tab[col_linhas-1][line];
			cout<<"Dest:"<<destroyed<<endl;

			//envia comando para socket
			char enviado[254];
			stringstream mensagem;
			mensagem<<"rodaX("<<(line+1)<<","<<
				toList2(tab)<<","<<0<<","<<
				1<<").\n";
			printf("MESSAGE IS:%s\n",(mensagem.str()).c_str());
			strcpy(enviado,(mensagem.str()).c_str());
			envia(enviado,strlen(enviado));
			char recebido[254];
			string resp;
			resp=recebe(recebido);
			cout<<"THIS IS RETURNED:\n-"<<resp<<endl;
			//examina resposta
			vector<vector<int> >new_tab ;
			if(resp.size()>3){
				new_tab = toVecVector2(resp);
				tab=new_tab;
			cout << "Valid command.prolog  answered: "/* << resp */<< endl;
			}
			else cout << "Incorrect command.prolog answered: " /*<< resp */<< endl;
		}
		else{
			int i=0,j=0;
			for(i=(tab.size()-1);i >0;i--){			
				for(j=0;j<tab[i].size();j++) {
					if(j==line){
						tab[i][j]=tab[i-1][j];
					}
				
				}
			}
			tab[i][line]=0;
		}

	}
	else if(dir==3){
		cout<<"Para esq"<<endl;
		if(!replay){
			if(keepDest==1)
				destroyed=tab[line][0];
		/*	cout<<"Dest:"<<destroyed<<endl;
			for(i=0; i <(tab[line].size()-1);i++){
				tab[line][i]=tab[line][i+1];
			}
			tab[line][i]=0;*/

			//envia comando para socket
			char enviado[254];
			stringstream mensagem;
			mensagem<<"rodaY("<<(line+1)<<","<<
				toList2(tab)<<","<<	2<<").\n";
			printf("MESSAGE IS:%s\n",(mensagem.str()).c_str());
			strcpy(enviado,(mensagem.str()).c_str());
			envia(enviado,strlen(enviado));
			char recebido[254];
			string resp;
			resp=recebe(recebido);
			cout<<"THIS IS RETURNED:\n-"<<resp<<endl;
			//examina resposta
			vector<vector<int> >new_tab ;
			if(resp.size()>3){
				new_tab = toVecVector2(resp);
	
				tab=new_tab;

			string answer2=toList2(new_tab);
			cout << "Valid command.prolog  answered: "/* << resp */<< endl;
			}
			else cout << "Incorrect command.prolog answered: " /*<< resp */<< endl;
		
			//Fim....
		}
		else{int i=0;
			for(i=0; i <(tab[line].size()-1);i++){
				tab[line][i]=tab[line][i+1];
			}
			tab[line][i]=0;

		}
	}
	else if(dir==4){
		cout<<"Para dir"<<endl;
		if(!replay){
			if(keepDest==1)
				destroyed=tab[line][col_linhas-1];
			cout<<"Dest:"<<destroyed<<endl;
			/*for(i=tab[line].size()-1; i>0;i--){
				tab[line][i]=tab[line][i-1];
			}
			tab[line][0]=0;*/


			//envia comando para socket
			char enviado[254];
			stringstream mensagem;
			mensagem<<"rodaY("<<(line+1)<<","<<
				toList2(tab)<<","<<	1<<").\n";
			printf("MESSAGE IS:%s\n",(mensagem.str()).c_str());
			strcpy(enviado,(mensagem.str()).c_str());
			envia(enviado,strlen(enviado));
			char recebido[254];
			string resp;
			resp=recebe(recebido);
			cout<<"THIS IS RETURNED:\n-"<<resp<<endl;
			//examina resposta
			vector<vector<int> >new_tab ;
			if(resp.size()>3){
				new_tab = toVecVector2(resp);
	
				tab=new_tab;

			string answer2=toList2(new_tab);
			cout << "Valid command.prolog  answered: "/* << resp */<< endl;
			}
			else cout << "Incorrect command.prolog answered: " /*<< resp */<< endl;
		
			//Fim....
		}
		else{int i=0;
			for(i=tab[line].size()-1; i>0;i--){
				tab[line][i]=tab[line][i-1];
			}
			tab[line][0]=0;

		}
	}
	if(!replay){
		//guardar jogada no vector jogada
		if(keepDest==1){ 
			vector<int> ints;
			ints.push_back(jogador);
			ints.push_back(dir);
			ints.push_back(line);
			ints.push_back(destroyed);

			string operacao=RODAR;
			pair <string ,vector<int>> play;
			play = make_pair (operacao,ints);

			jogadas.push_back(play);
		}
		//-------------------
		//if(double 
		if(doubleAi){
			if(!verificarFimJogo()){
				if(doubleAi){
					if(jogadorAI==2)
						jogadorAI=1;
					else{
						jogadorAI=2;
					}
					glutTimerFunc(mili_secs,goAIplay,150);
				}
			}
		}
	}


	/*
	pair <int,int> pos;
	pos.first=dir;
	pos.second=line;
	pair <string, pair <int,int> > play;
	play.first="rotacao";
	play.second=pos;
	jogadas.push_back(play);*/

	selected=false;
	turning=-1;
	cout<<"Jogador que jogou no turn!"<<jogador<<endl;
	if(!replay){
		verificarFimJogo();
	}

	//calcula_jogada(Tab,Nivel,Jogador)
	//insere_pecaY(Y,X,Jogador,Tab)
	if(!replay){
		if(AIlevel>0&&!doubleAi){
			AIplay();
		}
		else{
			if(jogador==1){
				if(ccamera==1){
				camera_step=0;
				glui2->disable();
				view_rotate[0]=0;view_rotate[1]=0;view_rotate[2]=1;view_rotate[3]=0;
				view_rotate[4]=0;view_rotate[5]=1;view_rotate[6]=0;view_rotate[7]=0;
				view_rotate[8]=-1;view_rotate[9]=0;view_rotate[10]=0;view_rotate[11]=0;
				view_rotate[12]=0;view_rotate[13]=0;view_rotate[14]=0;view_rotate[15]=1;
				glutTimerFunc(mili_secs,updateCamera1,0);
				}
				jogador=2;
			}
			else{
				if(ccamera==1){
				camera_step=0;
				glui2->disable();
				view_rotate[0]=0;view_rotate[1]=0;view_rotate[2]=-1;view_rotate[3]=0;
				view_rotate[4]=0;view_rotate[5]=1;view_rotate[6]=0;view_rotate[7]=0;
				view_rotate[8]=1;view_rotate[9]=0;view_rotate[10]=0;view_rotate[11]=0;
				view_rotate[12]=0;view_rotate[13]=0;view_rotate[14]=0;view_rotate[15]=1;
				glutTimerFunc(mili_secs,updateCamera2,0);
				}
				jogador=1;
			}
			}
	}
	else{
		glutTimerFunc(mili_secs,nextReplay,0); 
	}

}


void undo(){
	if(win==-1){
			int size=jogadas.size();
			if(size>=2){
				int counter=1;
				while(counter<3){
					cout<<"I'm in"<<endl;
					cout<<"SIZE IS:"<<jogadas.size()<<endl;
					pair<string, vector<int> >  last_play=jogadas[size-counter];cout<<"2"<<endl;
					string tipo=last_play.first;cout<<"2"<<endl;
					vector<int> jogada=last_play.second;cout<<"2"<<endl;
					int _jogador=jogada[0];cout<<"2"<<endl;

			
					if(tipo==INSERIR){
						cout<<"Undo de inserir"<<endl;
						int _posZ=jogada[1];
						int _posX=jogada[2];
						cout<<"---> na posicao:"<<_posZ<<" vs " <<_posX<<endl;
						tab[_posZ][_posX]=0;
						cout<<"DID"<<endl;
		
					}
					else if(tipo==RODAR){
				
						int direc=jogada[1];
						int col_linha=jogada[2];
						int pecaDest=jogada[3];
						switch(direc){
						case 1:
							turn(2,col_linha,0);
							tab[0][col_linha]=pecaDest;
							break;
						case 2:
					
							turn(1,col_linha,0);
							tab[col_linhas-1][col_linha]=pecaDest;
							break;
						case 3:
					
							turn(4,col_linha,0);
							tab[col_linha][0]=pecaDest;
							break;
						case 4:
					
							turn(3,col_linha,0);
							tab[col_linha][col_linhas-1]=pecaDest;
							break;
						default:
							cout<<"desconhecido"<<endl;


						}
					}
					else{
						cout<<"Turn: Unknown type"<<endl;
					}
					cout<<"erase"<<endl;
					cout<<(size-counter)<<endl;
					cout<<jogadas.size()<<endl;
					jogadas.erase(jogadas.begin()+(size-counter));
					cout<<"erased"<<endl;
					counter++;
				}
		}
		else
			cout<<"Jogadas insuficientes para fazer undo"<<endl;
	}
	else
		cout<<"Nao pode ser feito undo apos vitoria"<<endl;
}

void new_game(){
	win=-1;
	clear_vector();
	replay=false;
	jogador=1;
	vector<pair<string, vector<int> > >  new_jogadas;
	jogadas=new_jogadas;
}

void gStart(){
	new_game();
	if(doubleAi>0){
		AIlevel=AIlevel+1;
		doubleAi=doubleAi-1;
	}
	else{
		AIlevel=0;
	}
	cout<<AIlevel<<" "<<doubleAi<<endl;
	gameStart=true;
	glui3->hide();
	glui4->enable();
	if(ccamera==1){
	glui2->disable();
	view_rotate[0]=1;view_rotate[1]=0;view_rotate[2]=0;view_rotate[3]=0;
	view_rotate[4]=0;view_rotate[5]=1;view_rotate[6]=0;view_rotate[7]=0;
	view_rotate[8]=0;view_rotate[9]=0;view_rotate[10]=1;view_rotate[11]=0;
	view_rotate[12]=0;view_rotate[13]=0;view_rotate[14]=0;view_rotate[15]=1;
	camera_step=1;
	glutTimerFunc(mili_secs, updateCamera2, 0);
	}
	if(doubleAi==1){
		jogadorAI=1;
		AIplay();
		
	}
	else
		jogadorAI=2;
}
void validateStart()
{
	if(createStart==0)
		{
	glui3 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM);
	glui3->set_main_gfx_window( main_window );

	glui3->add_column( false );
	GLUI_Button *gstart = glui3->add_button("Start Game",0, (GLUI_Update_CB)gStart );
		
		glui3->add_column( false );
	GLUI_Panel *obj_panel1 = glui3->add_panel("Dificuldade");
	GLUI_RadioGroup *group1 = glui3->add_radiogroup_to_panel(obj_panel1,&AIlevel,4);
		glui3->add_radiobutton_to_group(group1, "Facil");
		glui3->add_radiobutton_to_group(group1, "Medio");
		glui3->add_radiobutton_to_group(group1, "Dificil");
		
		glui3->add_column( false );
	GLUI_Panel *obj_panel2 = glui3->add_panel("Modo de Jogo");
	GLUI_RadioGroup *group2 = glui3->add_radiogroup_to_panel(obj_panel2,&doubleAi,4);
		glui3->add_radiobutton_to_group(group2, "P1vsP2");
		glui3->add_radiobutton_to_group(group2, "P1vsCPU");
		glui3->add_radiobutton_to_group(group2, "CPUvsCPU");
		createStart=1;
		}
		else if(createStart==1&&gameStart==true)
		{
		time (&start);
		createStart=2;
		}
		else if(createStart==2)
		{
			time (&tend);
			if(difftime (tend,start)>1)
			{
				char timet[10];
				timec=timec+1;
				itoa(timec,timet,10);
				clockt->set_text(timet);
				start=tend;
			}
		}
}

void lightsbar()
{
	int counter=0;
	//check->hidden=true;
	map<string, Light*>::iterator it;
	 for(it= scene1->map_luzes.begin();it!=scene1->map_luzes.end();it++){
		stringstream ss;//create a stringstream
		ss << "Road - Light number-"<<counter;
		string name=ss.str();
		char button[64];
		button[0]=0;
		strcat (button,name.c_str());
		check = glui2->add_checkbox(button, &scene1->map_luzes[(*it).first]->enabled );
		counter++;
	 }
	 counter=0;
	 for(it= scene2->map_luzes.begin();it!=scene2->map_luzes.end();it++){
		stringstream ss;//create a stringstream
		ss << "Cafe - Light number-"<<counter;
		string name=ss.str();

		char button1[64];
		strcpy (button1,name.c_str());
		check1 = glui2->add_checkbox(button1, &scene2->map_luzes[(*it).first]->enabled );
		counter++;
	 }
	 counter=0;
	 for(it= scene3->map_luzes.begin();it!=scene3->map_luzes.end();it++){
		stringstream ss;//create a stringstream
		ss << "Cinema - Light number-"<<counter;
		string name=ss.str();

		char button1[64];
		strcpy (button1,name.c_str());
		check1 = glui2->add_checkbox(button1, &scene3->map_luzes[(*it).first]->enabled );
		counter++;
	 }
}

void nextReplay(int dummy){
	
	replayAll(current_move++);
	display();
}

void changet()
{
	if(oldtheme!=temas)
	{
	if(temas==0) scene=scene1;
	if(temas==1) scene=scene2;
	if(temas==2) scene=scene3;
	processIllumination();
	oldtheme=temas;
	}
}


//double speed=3.0;
void updatePeca(int dummy){
	
	double raiseY=3.0;
	
	double dim=dimensao/2.;
	GLdouble initialz=-dim;
	GLdouble size=dimensao/col_linhas;

	double distFromTabX = abs(peca1X);
	double distFromTabZ =abs(abs(peca1Z)-abs( -dim));
	double distX;
	if(jogador==1)
			distX= -peca1X + (-dim + posX*size) + size/2.0;
	else
			distX= peca1X + (-dim + posX*size) + size/2.0;
	double distZ= abs(distFromTabZ) + abs(posZ*size) + size/2.0;

	double Yspeed=raiseY/mili_secs;
	//double Xspeed=distX/mili_secs;
	//double Zspeed=distZ/mili_secs;
	
	double Xspeed=2*distX/mili_secs;
	double Zspeed=2*distZ/mili_secs;

	double movePecaX;
	double movePecaY;
	double movePecaZ;
	bool reset=false;
	
	
	if(jogador==1){
			movePecaX=movePeca1X;
			movePecaY=movePeca1Y;
			movePecaZ=movePeca1Z;
		}
		else{
			movePecaX=movePeca2X;
			movePecaY=movePeca2Y;
			movePecaZ=movePeca2Z;
		}
	
	if(onTheMove)
	{
		
		if(faseCancel){
		
			movePecaY-=Yspeed;
			if(movePecaY<=peca1Y){
				faseCancel=false;
				onTheMove=false;
				selected=false;
			}
		}
		else if(fase1){
			
			movePecaY+=Yspeed;
			if(movePecaY>=raiseY){
				
				fase1=false;
				onTheMove=false;
				if(vezAI){
					fase2=true;
					onTheMove=true;
				}
				if(replay){
					cout<<"GOT TO FASE 1"<<endl;
					onTheMove=true;
					fase2=true;
				}
			}

		}
		else if(fase2){
			
			bool movi=false;
			if(abs(movePecaX)< abs(distX)){
				movePecaX+=Xspeed;
				if(abs(movePecaX)> abs(distX)){
					movePecaX=distX;
				}
				movi=true;
			}
			
			if(movePecaZ<distZ){
				movePecaZ+=Zspeed;
				if(movePecaZ>distZ)
					movePecaZ=distZ;
				movi=true;
			}

			if(!movi){
				
				fase2=false;
				fase3=true;
				
			}

		}
		else if(fase3){
			Yspeed=(raiseY-(dimensaoY))/mili_secs;
			movePecaY-=Yspeed;
			if(movePecaY<=dimensaoY){
				fase3=false;
				onTheMove=false;
				selected=false;
				//cout<<posZ<<" inseri"<<posX<<endl;
				//cout<<posZ<<" vs "<<posX<<endl;
			//	tab[posX][posZ]=1;
				
				inserirPeca(posZ,posX,jogador);
			/*	peca1X=iniPeca1X;
				peca1Y=iniPeca1Y;
				peca1Z=iniPeca1Z;*/
				
				reset=true;
			}

		}
		if(reset){
		
			movePecaX=0;
			movePecaY=0;
			movePecaZ=0;

			if(AIlevel>0){
				if(jogador==1){
					jogador=2;
				}else{
					jogador=1;
				}
				if(vezAI)
					vezAI=false;
				else
					vezAI=true;
			}

				
				movePeca1X=movePecaX;
				movePeca1Y=movePecaY;
				movePeca1Z=movePecaZ;
				
				movePeca2X=movePecaX;
				movePeca2Y=movePecaY;
				movePeca2Z=movePecaZ;
			
			posZ=-1;
			posX=-1;
			if(replay){
				if(jogador==2){
					//cout<<"2entrei no jog==2 e pus isto:"<<movePecaX<<","<<movePecaY<<","<<movePecaZ<<endl;
					movePeca2X=movePecaX;
					movePeca2Y=movePecaY;
					movePeca2Z=movePecaZ;
				}
				else{
					//cout<<"2entrei e jog==1 pus isto:"<<movePecaX<<","<<movePecaY<<","<<movePecaZ<<endl;
					movePeca1X=movePecaX;
					movePeca1Y=movePecaY;
					movePeca1Z=movePecaZ;
				}
				glutTimerFunc(mili_secs,nextReplay,0); 
			}
			if(AIlevel>0&&vezAI&&!replay&&!doubleAi){
				
				glutTimerFunc(mili_secs,goAIplay,150);
		
				
			}
			else if(doubleAi){
				if(!verificarFimJogo()){
					if(doubleAi){
						if(jogadorAI==2)
							jogadorAI=1;
						else{
							jogadorAI=2;
						}
						glutTimerFunc(mili_secs,goAIplay,150);
					}
				}

			}
			
		
		
		}
		else {
			if(jogador==1){
				movePeca1X=movePecaX;
				movePeca1Y=movePecaY;
				movePeca1Z=movePecaZ;
			}
			else{
				movePeca2X=movePecaX;
				movePeca2Y=movePecaY;
				movePeca2Z=movePecaZ;
			}
		}

		
		/*cout<<"-----------------------"<<endl;
		cout<<movePeca1X<<endl;
		cout<<movePeca1Y<<endl;
		cout<<movePeca1Z<<endl;
		cout<<"++++++++++++++++++++++++++"<<endl;
		cout<<movePeca2X<<endl;
		cout<<movePeca2Y<<endl;
		cout<<movePeca2Z<<endl;
		cout<<"-----------------------"<<endl;*/
		if(onTheMove)
			glutTimerFunc(mili_secs,updatePeca,0);
		
	}
	else{
		cout<<"\n\n tou la mas nao ha onthemove"<<endl;
	}


}

void updatePecaOLD(int dummy){

	double raiseY=3.0;
	
	double dim=dimensao/2.;
	GLdouble initialz=-dim;
	GLdouble size=dimensao/col_linhas;

	double distFromTabX = abs(peca1X);
	double distFromTabZ =abs(abs(peca1Z)-abs( -dim));
	double distX;
	if(jogador==1)
			distX= -peca1X + (-dim + posX*size) + size/2.0;
	else
			distX= peca1X + (-dim + posX*size) + size/2.0;
	double distZ= abs(distFromTabZ) + abs(posZ*size) + size/2.0;

	double Yspeed=raiseY/mili_secs;
	double Xspeed=distX/mili_secs;
	double Zspeed=distZ/mili_secs;

	double movePecaX;
	double movePecaY;
	double movePecaZ;
	bool reset=false;


	if(jogador==1){
			movePecaX=movePeca1X;
			movePecaY=movePeca1Y;
			movePecaZ=movePeca1Z;
		}
		else{
			movePecaX=movePeca2X;
			movePecaY=movePeca2Y;
			movePecaZ=movePeca2Z;
		}
	if(onTheMove)
	{
		if(faseCancel){
			movePecaY-=Yspeed;
			if(movePecaY<=peca1Y){
				faseCancel=false;
				onTheMove=false;
				selected=false;
			}
		}
		else if(fase1){
			movePecaY+=Yspeed;
			if(movePecaY>=raiseY){
				
				fase1=false;
				onTheMove=false;
			}

		}
		else if(fase2){
			bool movi=false;
			if(abs(movePecaX)< abs(distX)){
				movePecaX+=Xspeed;
				movi=true;
			}
			
			if(movePecaZ<distZ){
				movePecaZ+=Zspeed;
				movi=true;
			}

			if(!movi){
				
				fase2=false;
				fase3=true;
				
			}

		}
		else if(fase3){
			Yspeed=(raiseY-(dimensaoY))/mili_secs;
			movePecaY-=Yspeed;
			if(movePecaY<=dimensaoY){
				fase3=false;
				onTheMove=false;
				selected=false;
				//cout<<posZ<<" inseri"<<posX<<endl;
				//cout<<posZ<<" vs "<<posX<<endl;
			//	tab[posX][posZ]=1;
				inserirPeca(posZ,posX,jogador);
			/*	peca1X=iniPeca1X;
				peca1Y=iniPeca1Y;
				peca1Z=iniPeca1Z;*/
				reset=true;
			}

		}
		if(reset){
			movePecaX=0;
			movePecaY=0;
			movePecaZ=0;

			if(jogador==2){
				movePeca1X=movePecaX;
				movePeca1Y=movePecaY;
				movePeca1Z=movePecaZ;
			}
			else{
				movePeca2X=movePecaX;
				movePeca2Y=movePecaY;
				movePeca2Z=movePecaZ;
			}
		
		}
		else {
			if(jogador==1){
				movePeca1X=movePecaX;
				movePeca1Y=movePecaY;
				movePeca1Z=movePecaZ;
			}
			else{
				movePeca2X=movePecaX;
				movePeca2Y=movePecaY;
				movePeca2Z=movePecaZ;
			}
		}

		
		/*cout<<"-----------------------"<<endl;
		cout<<movePeca1X<<endl;
		cout<<movePeca1Y<<endl;
		cout<<movePeca1Z<<endl;
		cout<<"++++++++++++++++++++++++++"<<endl;
		cout<<movePeca2X<<endl;
		cout<<movePeca2Y<<endl;
		cout<<movePeca2Z<<endl;
		cout<<"-----------------------"<<endl;*/
		if(onTheMove)
			glutTimerFunc(mili_secs,updatePeca,0);
		
	}


}
void movePeca(){

	//insere_pecaY(Y,X,Jogador,Tab)
	
	if(posZ>=0&&posX>=0){
		if(!replay){
			
			//envia comando para socket
			char enviado[254];
			stringstream mensagem;
			mensagem<<"insere_pecaY("<<(posZ+1)<<","<<(posX+1)<<","<<
			jogador<<","<<toList2(tab)<<").\n";
			strcpy(enviado,(mensagem.str()).c_str());
			envia(enviado,strlen(enviado));
			char recebido[254];
			string resp;
			resp=recebe(recebido);
		
			//examina resposta
			vector<vector<int> >new_tab ;

			if(resp.size()>6){
			
			
			
				onTheMove=true;
				glutTimerFunc(mili_secs, updatePeca, 0);
				string answer2=toList2(new_tab);
				cout << "Valid command."<< endl;
			}
			else{
				cout << "Incorrect command" << endl;
			}
			//Fim....
		}
		else{
			cout<<"chamei o updatePeca"<<endl;
			onTheMove=true;
			glutTimerFunc(mili_secs, updatePeca, 150);
		}
	}else{
		onTheMove=true;
		glutTimerFunc(mili_secs, updatePeca, 0);
	}


	//onTheMove=true;
	//glutTimerFunc(mili_secs, updatePeca, 0);
}
void replayOne(pair<string, vector<int> > _jogadaRepr){
	/*

	1ªstring="inserir" ou rodar"
	vector<int>=
		int[0]=jogador;
		se "inserir" = int[1]=Y e int[2]=X  <=> posicao(Y;X) 
		se "rodar" = int[1]=Direcao e int[2]=Linha/Coluna int[3]=PecaDestruida
	rodar: int é 1-rodar para cima, 2-baixo, 3-esquerda,4-direita
	*/
	
	
	string tipo=_jogadaRepr.first;
	cout<<"tipo-"<<tipo<<endl;
	vector<int> jogada=_jogadaRepr.second;
	int _jogador=jogada[0];
	cout<<"jogador-"<<_jogador<<endl;

			
	if(tipo==INSERIR){
		cout<<"Replay de inserir"<<endl;
		jogador=_jogador;
		posZ=jogada[1];
		posX=jogada[2];
		cout<<"---> na posicao:"<<posZ<<" vs " <<posX<<endl;
		//tab[_posZ][_posX]=0;
		movePeca();
		fase1=true;
		selected=true;
				
	}
	else if(tipo==RODAR){
		cout<<"Replay de rodar"<<endl;
		clock_t wait;
		wait = clock();
		while( clock() != wait + 500 );
		int direc=jogada[1];
		int col_linha=jogada[2];
		int pecaDest=jogada[3];
		switch(direc){
		case 1:cout<<"cima"<<endl;
			turn(1,col_linha,0);
			break;
		case 2:
			cout<<"baixo"<<endl;
			turn(2,col_linha,0);
			break;
		case 3:
			cout<<"esq"<<endl;		
			turn(3,col_linha,0);
			break;
		case 4:
			cout<<"dir"<<endl;		
			turn(4,col_linha,0);
			break;
		default:
			cout<<"desconhecido"<<endl;


		}
	}
	else{
		cout<<"Turn: Unknown type"<<endl;
	}
	

}


//vector<pair<string,pair<int,int> > > jogadas //string -tipo("jogada","rotacao")- se rotacao: primeiro int é 1-rodar para cima, 2-baixo, 3-esquerda,4-direita. segundo int:linha ou coluna a rodar
void highlightLines(){
	double dim=dimensao/2.;
	GLdouble initial=-dim;
	GLdouble size=dimensao/col_linhas;
	
	vector<int> colunasPreenchidas(col_linhas,false);//colunas com numeros que podem ser rodadas
	glEnable(GL_COLOR_MATERIAL);	
	float mat1_emission[] = {0.4, 0.4, 0.0, 0.0};
	float mat1_specular[] = {0.4, 0.4, 0.0, 1.0};	/* specular reflection. */
	float mat1_diffuse[] =  {0.4, 0.4, 0.0, 1.0};	/* diffuse reflection. */
	float mat1_ambient[] =  {0.4, 0.4, 0.0, 1.0};	/* ambient reflection. */
	float mat1_shininess[] =  {20.0};

	//cout<<id<<" is giving value "<<material.second.emr<<endl;//" "<<material.second.difg<<" "<<material.second.difb<<" "<<material.second.difa<<endl;

	glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION,  mat1_emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat1_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat1_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat1_ambient);
	int i,j;
	for(i=0;i<tab.size();i++){
			bool linhaPreenchida=false;
			for(j=0;j<tab[i].size();j++){
				if(tab[i][j]!=0){
					linhaPreenchida=true;
					colunasPreenchidas[j]=true;
				}
			}

			//desenha esfera no fim e inicio da linha preenchida
			if(linhaPreenchida){
				GLdouble desloc1=initial+i*size+size/2.0;


						glPushName (53);//codigo paras as rodagens para a cima
						glPushName (i);
					
				//esfera da esquerda
				glPushMatrix();
				glTranslatef(-dim-1,0,desloc1);
				gluSphere(glQ,0.5,20,8);
				glPopMatrix();
				
					glPopName ();
					glPopName ();
				
				
						glPushName (54);//codigo paras as rodagens para a baixo
						glPushName (i);
				
				//esfera da esquerda
				glPushMatrix();
				glTranslatef(dim+1,0, desloc1);
				gluSphere(glQ,0.5,20,8);
				glPopMatrix();
				
						glPopName ();
						glPopName ();
				
			}
	}
		
	for(i=0;i<colunasPreenchidas.size();i++){
		//desenha esfera no fim e inicio da coluna preenchida
			if(colunasPreenchidas[i]){
				GLdouble desloc1=initial+i*size+size/2.0;
				

					
						glPushName (51);//codigo paras as rodagens para a esquerda
						glPushName (i);
					
				//esfera da esquerda
				glPushMatrix();
				glTranslatef(desloc1,0, -dim-1);
				gluSphere(glQ,0.5,20,8);
				glPopMatrix();
			
						glPopName ();
						glPopName ();
				
			
						glPushName (52);//codigo paras as rodagens para a direita
						glPushName (i);
				
				//esfera da esquerda
				glPushMatrix();
				glTranslatef(desloc1 ,0,+dim+1);
				gluSphere(glQ,0.5,20,8);
				glPopMatrix();
				
						glPopName ();
						glPopName ();
				
			}
	}
	
}


void paralelo(GLdouble dimx, GLdouble dimy, GLdouble dimz){
	GLdouble dx=dimx/2, dy=dimy/2, dz=dimz/2;
	
	GLdouble v1[3] = {dx,-dy,dz};
	GLdouble v2[3] = {dx,-dy,-dz};
	GLdouble v3[3] = {dx,dy,dz};
	GLdouble v4[3] = {dx,dy,-dz};
	GLdouble v5[3] = {-dx,-dy,dz};
	GLdouble v6[3] = {-dx,dy,dz};
	GLdouble v7[3] = {-dx,dy,-dz};
	GLdouble v8[3] = {-dx,-dy,-dz};
	GLdouble normal[VLENGTH];

	float mat_shininess[] = {20.0}; /* How shiny is the object (specular exponent)  */
	float mat_specular[] = {0.5, 0.5, 0.5, 1.0}; /* specular reflection. */
	float mat_diffuse[] = {0.5, 0.5, 0.5, 1.0}; /* diffuse reflection. */
	// define as caracteristicas do material (dos materiais seguintes, i.e. ate nova alteracao
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);


	//Face frente - 0
	newellSquare(v1,v3,v6,v5,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v1);
		glVertex3dv(v3);
		glVertex3dv(v6);
		glVertex3dv(v5);
	glEnd();

	// face anterior - 1
	newellSquare(v8,v7,v4,v2,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v8);
		glVertex3dv(v7);
		glVertex3dv(v4);
		glVertex3dv(v2);
	glEnd();

	// face lateral - 2
	newellSquare(v2,v4,v3,v1,normal);
	glBegin(GL_POLYGON);
 	    glNormal3dv(normal);
		glVertex3dv(v2);
		glVertex3dv(v4);
		glVertex3dv(v3);
		glVertex3dv(v1);
	glEnd();

	newellSquare(v5,v6,v7,v8,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v5);
		glVertex3dv(v6);
		glVertex3dv(v7);
		glVertex3dv(v8);
	glEnd();

	// base
	newellSquare(v1,v5,v8,v2,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v1);
		glVertex3dv(v5);
		glVertex3dv(v8);
		glVertex3dv(v2);
	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2014);			// activa a textura 2001

	// topo 
	newellSquare(v3,v4,v7,v6,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0); glVertex3dv(v3);
		glTexCoord2f(1.0,0.0); glVertex3dv(v4);
		glTexCoord2f(1.0,1.0); glVertex3dv(v7);
		glTexCoord2f(0.0,1.0); glVertex3dv(v6);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void mesa(float dimy)
{
		float mat_shininess[] = {20.0}; /* How shiny is the object (specular exponent)  */
	float mat_specular[] = {0.3, 0.3, 0.3, 1.0}; /* specular reflection. */
	float mat_diffuse[] = {0.5, 0.5, 0.5, 1.0}; /* diffuse reflection. */
	// define as caracteristicas do material (dos materiais seguintes, i.e. ate nova alteracao
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 2013);	
	glPushMatrix();
	glTranslatef(0,-((dimy/2.0)+0.05),0);
	glRotatef(-90,1,0,0);
	gluDisk(glQ,0,12,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,-6.5,0);
	glRotatef(-90,1,0,0);
	gluCylinder(glQ,2,2,6,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,-6.5,0);
	glRotatef(-90,1,0,0);
	gluDisk(glQ,0,2,20,20);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void woodBox(float dsize)
{
	glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 2013);			// activa a textura 2001
  glPushMatrix();
  glNormal3f(1.0,0.0, 0.0);       
  glTranslated(-2*dsize,0.0,0.0);
  glRotated(-90, 0.0,1.0,0.0);
  glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-dsize, dsize, 0);	// associacao de
  glTexCoord2f(1.0,0.0);  glVertex3d(-dsize, -dsize, 0);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(dsize, -dsize, 0);	// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(dsize, dsize,  0);	// vertices 3D do poligono
  glEnd(); 
  glPopMatrix();
	
	glPushMatrix();
  glNormal3f(0.0,0.0, -1.0);       
  glTranslated(0.0,0.0,-dsize);
  glRotated(180, 0.0,1.0,0.0);
   glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-2*dsize, dsize, 0);	// associacao de
  glTexCoord2f(1.0,0.0);  glVertex3d(-2*dsize, -dsize, 0);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(2*dsize, -dsize, 0);	// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(2*dsize, dsize,  0);	// vertices 3D do poligono
  glEnd();
  glPopMatrix();
  //frente
  glPushMatrix();
  glNormal3f(0.0,0.0, 1.0);       
  glTranslated(0.0,0.0,dsize);
  glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-2*dsize, dsize, 0);	// associacao de
  glTexCoord2f(1.0,0.0);  glVertex3d(-2*dsize, -dsize, 0);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(2*dsize, -dsize, 0);	// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(2*dsize, dsize,  0);	// vertices 3D do poligono
  glEnd();
  glPopMatrix();
 
  glPushMatrix();
  glNormal3f(0.0,1.0,0.0);       
  glTranslated(0.0,dsize,0.0);
  glRotated(-90, 1.0,0.0,0.0);
   glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-2*dsize, dsize, 0);	// associacao de
  glTexCoord2f(1.0,0.0);  glVertex3d(-2*dsize, -dsize, 0);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(2*dsize, -dsize, 0);	// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(2*dsize, dsize,  0);	// vertices 3D do poligono
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glNormal3f(-1.0,0.0, 0.0);       
  glTranslated(2*dsize,0.0,0.0);
  glRotated(90, 0.0,1.0,0.0);
   glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-dsize, dsize, 0);	// associacao de
  glTexCoord2f(1.0,0.0);  glVertex3d(-dsize, -dsize, 0);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(dsize, -dsize, 0);	// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(dsize, dsize,  0);	// vertices 3D do poligono
  glEnd(); 
  glPopMatrix();

  glPushMatrix();
  glNormal3f(0.0,-1.0, 0.0);
  glTranslated(0.0,-dsize,0.0);
  glRotated(90, 1.0,0.0,0.0);
   glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-2*dsize, dsize, 0);	// associacao de
  glTexCoord2f(1.0,0.0);  glVertex3d(-2*dsize, -dsize, 0);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(2*dsize, -dsize, 0);	// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(2*dsize, dsize,  0);	// vertices 3D do poligono
  glEnd();
  glPopMatrix();
	 //SCORE
  	
	
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 2012);			// activa a textura 2001
  glPushMatrix();
  glNormal3f(0.0,0.0, 1.0);       
  glTranslated(0.0,0.0,dsize);
   glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);   glVertex3d(-2*dsize, 0, 0.05);// associacao de
  glTexCoord2f(1.0,0.0); glVertex3d(2*dsize, 0, 0.05);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(2*dsize, dsize,  0.05);		// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(-2*dsize, dsize, 0.05);	// vertices 3D do poligono
  glEnd();
  glPopMatrix();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, scoreJog2+2003);			// activa a textura 2001
  //esq
  glPushMatrix();
  glNormal3f(0.0,0.0, 1.0);       
  glTranslated(0.0,0.0,dsize);
  glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-dsize, -dsize, 0.05);		// associacao de
  glTexCoord2f(1.0,0.0); glVertex3d(0-(dsize/2.0), -dsize, 0.05);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(0-(dsize/2.0), 0,  0.05);// das texturas, aos
  glTexCoord2f(0.0,1.0);  glVertex3d(-dsize, 0, 0.05);	// vertices 3D do poligono
  glEnd();
  glPopMatrix();
  	

  //dir
	 glBindTexture(GL_TEXTURE_2D, scoreJog1+2003);
    glPushMatrix();
  glNormal3f(0.0,0.0, 1.0);       
  glTranslated(0.0,0.0,dsize);
  glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d((dsize/2.0), -dsize, 0.05);	// associacao de
  glTexCoord2f(1.0,0.0); 	glVertex3d(dsize, -dsize, 0.05);// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(dsize, 0,  0.05);	// das texturas, aos
  glTexCoord2f(0.0,1.0);  glVertex3d((dsize/2.0), 0, 0.05);	// vertices 3D do poligono
  glEnd();
  glPopMatrix();
  	glDisable(GL_TEXTURE_2D);
  //--------------
}

float dsize=1.25;

void scoreBox()
{
	
	woodBox(dsize);

	/*glPushMatrix();
  glNormal3f(0.0,0.0, 1.0);       
  glTranslated(0.0,0.0,dsize);
   glBegin(GL_POLYGON); 
  glTexCoord2f(0.0,0.0);  glVertex3d(-dsize, dsize, 0);	// associacao de
  glTexCoord2f(1.0,0.0);  glVertex3d(-dsize, -dsize, 0);	// coordenadas u,v
  glTexCoord2f(1.0,1.0); glVertex3d(dsize, -dsize, 0);	// das texturas, aos
  glTexCoord2f(0.0,1.0); glVertex3d(dsize, dsize,  0);	// vertices 3D do poligono
  glEnd();*/
}


void drawScene(GLenum mode)
{

	float mat1_ambient[] =  {0.3, 0.3, 0.3, 1.0};	/* ambient reflection. */
	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat1_ambient);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glTranslated(0.0,0.0,-30.0);
	glRotated(45.0, 1.0,0.0,0.0 );
    glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
	glMultMatrixf( view_rotate );
	


	glPushMatrix();
	glTranslatef(0,dsize/2+0.5,iniPeca1Z);
	scoreBox();
	glPopMatrix();
	mesa(1.0);

	// tabuleiro
	if (mode == GL_SELECT)
		glLoadName (0);	
	glCallList(mesaList);
	
	if (mode == GL_SELECT)
	{
		glLoadName (49);
		glPushMatrix();
		glTranslated(0, 0.5, 0);
		squares(dimensao,dimensaoY,dimensao);
		glPopMatrix();
		glPopName();
		
	}
	glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	highlightLines();
	glPushMatrix();
	glTranslated(-dimensao/2.0, dimensaoY, -dimensao/2.0);
	draw_pecas();
	glPopMatrix();

	
	// Peca
	if (mode == GL_SELECT)
	{
		glPushName (1);
	}

	glPushMatrix();
	glTranslatef(movePeca1X,movePeca1Y,movePeca1Z);
	glTranslatef(peca1X,peca1Y,peca1Z);
	peca(radiusPeca,1);
	glPopMatrix();



	glDisable(GL_COLOR_MATERIAL);
	if (mode == GL_SELECT)
	{
		glPopName ();
	}

	if (mode == GL_SELECT)
	{
		glPushName (2);
	}

	glPushMatrix();
	glTranslatef(movePeca2X,movePeca2Y,movePeca2Z);
	glTranslatef(peca2X,peca2Y,peca2Z);
	peca(radiusPeca,2);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	if (mode == GL_SELECT)
	{
		glPopName ();
	}

	glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_diffuse);

}


void display(void)
{
	if(win==-1)validateStart();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*for(int i=0;i<16;i++)
	{
		cout<<view_rotate[i]<<" ";
		if(i==3||i==7||i==11||i==15)
		{cout<<endl;}
	}
	cout<<endl<<endl<<endl;*/
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 500.0 );

	processLights();
	// swapping the buffers causes the rendering above to be shown

	drawScene(GL_RENDER);

	if(wireframe){
	
		glPolygonMode(GL_FRONT, GL_LINE );	// preence a face da frente dos poligonos
	}
	else{
		glPolygonMode(GL_FRONT, GL_FILL);
	}


	No* root = scene->map_componentes[scene->root_id];
	glPushMatrix();
	glTranslatef(0,-6.5,0);
	root->draw();
	glPopMatrix();

	glutSwapBuffers();
   
	glFlush();
}

void replayAll(int playtoreplay){
	
	if(playtoreplay<jogadas.size()){
	

		pair<string, vector<int> > this_jogada;
		vector<pair<string, vector<int> > > this_vector=jogadas;
		this_jogada=jogadas[playtoreplay];
		replayOne(this_jogada);

	}
	else{
		cout<<"done replaying"<<endl;
	}
}
void secondReplay(){
	if(win!=-1){
		cout<<"current_move:"<<current_move<<endl;
		if(current_move>=jogadas.size()||current_move==0){
			current_move=0;
			replay=true;
			vector<vector<int> > this_tab;
			vector<int> inner (col_linhas,0);
			int i=0;
			for(i=0;i<col_linhas;i++){
				this_tab.push_back(inner);
			}
			tab=this_tab;
			display();
			glutTimerFunc(mili_secs, nextReplay, 0);
		}
		
	}
	//}
}
void processHits (GLint hits, GLuint buffer[])
{
	int i;
	unsigned int j;
    GLuint nnames, *ptr;
	GLuint selected_nnames, *selected_ptr;
	unsigned int z1=0, z2=0;
	unsigned int selected_z1, selected_z2;

	pk->resetEstados();
	if (hits == 0)
		return;

    printf ("hits = %d\n", hits);
    ptr = (GLuint *) buffer;

    for (i = 0; i < hits; i++) 
	{	/*  for each hit  */
		nnames = *ptr; ptr++;
		z1 = (unsigned int)*ptr; ptr++;
		z2 = (unsigned int)*ptr; ptr++;

		if ((i == 0) || (z1 < selected_z1))
		{
			selected_nnames=nnames;
			selected_z1=z1;
			selected_z2=z2;
			selected_ptr=ptr;
		}
		
		printf ("nbr_names=%u;", nnames);
		printf (" z1=%u;", z1);
		printf (" z2=%u;", z2);
		
		printf (" quem:");
		for (j = 0; j < nnames; j++)
		{
			printf (" %d", *ptr);
			ptr++;
		}
		printf ("\n");
	}

	///PECA Selecionada
	if(selected&&turning==-1){
		double tabPosX=-1;
		double tabPosZ=-1;
	//	bool tabSelected=false;
		for (j = 0; j < selected_nnames; j++)
		{	
			if(!onTheMove&&selected_nnames==1&& j==0 &&(*selected_ptr==jogador)){
				cout<<"Selecao cancelada"<<endl;

				faseCancel=true;
				movePeca();
				//selected=false;
			}
			else if(!onTheMove&&selected_nnames==3&& j==0 &&
				*selected_ptr==49){
					tabPosX=*selected_ptr;
					
			}
			else if(!onTheMove&&selected_nnames==3&& j==1 &&
				*selected_ptr>=0&&*selected_ptr<=7){
					tabPosX=*selected_ptr;
			}
			else if(!onTheMove&&selected_nnames==3&& j==2 &&
				*selected_ptr>=0&&*selected_ptr<=7){
					tabPosZ=*selected_ptr;
			}
			printf (" %d", *selected_ptr);
			selected_ptr++;
		}
		if(tabPosX!=-1&&tabPosZ!=-1){
			cout<<"encontrei tab"<<endl;
			posX=tabPosX;
			posZ=tabPosZ;
			cout<<"HEY! its: "<<posZ<<" and "<<posX<<endl;
			fase2=true;
			movePeca();
		}
	}//Escolher peca ou rotacao
	else if(!selected){
		double tempPosX=-1;
		double tempPosZ=-1;
		
		double turn_pos=-1;
		for (j = 0; j < selected_nnames; j++)
		{
			//peca a jogar
			if(selected_nnames==1&& j==0 &&*selected_ptr==1&&jogador==1){
				cout<<"bem seleccionado Peca 1"<<endl;

				movePeca();
				fase1=true;
				selected=true;
			}
			else if(selected_nnames==1&& j==0 &&*selected_ptr==2&&jogador==2){
				cout<<"bem seleccionado Peca 2"<<endl;

				movePeca();
				fase1=true;
				selected=true;
			}
			else if(selected_nnames==3&& j==1 &&
				*selected_ptr>=0&&*selected_ptr<=7){
					tempPosZ=*selected_ptr;
			}
			else if(turning!=-1&&selected_nnames==2){
				turn_pos=*selected_ptr;
			}
			else if(selected_nnames==2&& j==0){

				turning=*selected_ptr;
				if(turning>50&&turning<55){
					//cout<<"turning is on"<<endl<<endl;
					selected=true;
					//cout<<"turn is:"<<turning<<endl;
				}
			}

			printf (" %d", *selected_ptr);
			selected_ptr++;
		}
		if(turn_pos!=-1){
			if(turning==51){//cima
				//cout<<"."<<endl;
				turn(1,turn_pos,1);
			}
			else if(turning==52){//baixo
				//cout<<".."<<endl;
				turn(2,turn_pos,1);
			}
			else if(turning==53){//esquerda
				//cout<<"..."<<endl;
				turn(3,turn_pos,1);
			}
			else if(turning==54){//direita
				//cout<<"...."<<endl;
				turn(4,turn_pos,1);
			}
			
		}
	}
	

	selected_ptr--;
	pk->setEstado(*selected_ptr);
	dx = dxAc = pk->getDx(*selected_ptr);		// obtem dados da posicao actual do objecto
	dz = dzAc = pk->getDz(*selected_ptr);

}


/* Mouse handling */
void processMouse(int button, int state, int x, int y)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];

	Xini = x*view_rotate[0] - y*view_rotate[8];
	Yini = -x*view_rotate[2] + y*view_rotate[10];

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{	
		pk->setDxyz(dx, 0.0, dz);		// o objecto seleccionado esta registado em pk
	
		return;
	}

	glGetIntegerv (GL_VIEWPORT, viewport);

    glSelectBuffer (BUFSIZE, selectBuf);
    (void) glRenderMode (GL_SELECT);

    // inicia processo de picking
	glInitNames();
    glPushName(-1);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
	//  create 5x5 pixel picking region near cursor location
    gluPickMatrix ((GLdouble) x, (GLdouble) (window_h - y), 5.0, 5.0, viewport);

    glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 50.0 );

	drawScene(GL_SELECT);

    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glFlush ();

    hits = glRenderMode (GL_RENDER);
	if(win==(-1)&&doubleAi==0){
		processHits (hits, selectBuf);
	}
	

}
int getPeca(double x,double z){




	return -1;
}

void processMouseMoved(int x, int y)
{	int x1, y1;

	x1 = x*view_rotate[0] - y*view_rotate[8];
	y1 = -x*view_rotate[2] + y*view_rotate[10];

	dx = dxAc + (float)(x1 - Xini)/20.0;
	dz = dzAc + (float)(y1 - Yini)/20.0;

	// pedido de refrescamento da janela
	// glutPostRedisplay();				

}

void processPassiveMouseMoved(int x, int y)
{

	// pedido de refrescamento da janela
	glutPostRedisplay();				
}



void reshape(int w, int h)
{
	int tx, ty, tw, th;

	window_w = w;	//variaveis globais; window_h e' usado em processMouse()
	window_h = h;

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
	// ver, nos manuais, a funcao GLUI_Master.auto_set_viewport()

	xy_aspect = (float)tw / (float)th;

	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
switch (key) {
	  case 'q':
		light0y+= 0.1;
		cout << "light0y: " << light0y << endl; 
		break;
	  case 'a':
		light0y-= 0.1;
		cout << "light0y: " << light0y << endl; 
		break;
	  case 'i':
		light0_kc+=0.1;
		cout << "light0_kc: " << light0_kc << endl; 
		break;
	  case 'o':
		light0_kc-=0.1;
		cout << "light0_kc: " << light0_kc << endl; 
		break;
	  case 'k':
		light0_kl+=0.1;
		cout << "light0_kl: " << light0_kl << endl; 
		break;
	  case 'l':
		light0_kl-=0.1;
		cout << "light0_kl: " << light0_kl << endl; 
		break;
	  case 'n':
		light0_kq+=0.1;
		cout << "light0_kq: " << light0_kq << endl; 
		break;
	  case 'm':
		light0_kq-=0.1;
		cout << "light0_kq: " << light0_kq << endl; 
		break;
	  case 'z':
		divisoes_i+=3;
		cout << "div_i: " << divisoes_i <<endl;
		break;
	  case 'x':
		divisoes_i-=3;
		cout << "div_i: " << divisoes_i <<endl;
		break;
	  case 'c':
		divisoes_j+=5;
		cout << "div_j: " << divisoes_j <<endl;
		break;
	  case 'v':
		divisoes_j-=5;
		cout << "div_j: " << divisoes_j <<endl;
		break;

      case 27:		// tecla de escape termina o programa
		  executeOp(2);
         exit(0);
         break;
   }
	map<string, No*>::iterator it;
	for ( it = scene->map_componentes.begin() ; it != scene->map_componentes.end(); it++ ) 
	{
		if((*it).second->key!='\0')
		{
			if(key==(*it).second->key)
			{
				for(int i=0; i<(*it).second->materials.size();i++)
				{
					if((*it).second->material_id==(*it).second->materials[i])
						{
							if((i+1)<(*it).second->materials.size())
								{
									cout<<"changing "<<(*it).second->material_id<<" to "<<(*it).second->materials[i+1]<<endl;
									scene->map_componentes[(*it).first]->material_id=(*it).second->materials[i+1];
									//cout<<(*it).first<<" changed color"<<endl;
								}
							else  
								{
									cout<<"changing "<<(*it).second->material_id<<" to "<<(*it).second->materials[0]<<endl;
									scene->map_componentes[(*it).first]->material_id=(*it).second->materials[0];
								}
							break;
						}
				}
			}
		}
	}
}


void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  


  glutPostRedisplay();

  /****************************************************************/
  /*            This demonstrates GLUI::sync_live()               */
  /*   We change the value of a variable that is 'live' to some   */
  /*   control.  We then call sync_live, and the control          */
  /*   associated with that variable is automatically updated     */
  /*   with the new value.  This frees the programmer from having */
  /*   to always remember which variables are used by controls -  */
  /*   simply change whatever variables are necessary, then sync  */
  /*   the live ones all at once with a single call to sync_live  */
  /****************************************************************/

//  glui->sync_live();

}

void inicializacao()
{
	float ambient20[] = {0.5, 0.5, 0.5, 1.0};
	float diffuse20[] = {1.0, 1.0, 1.0, 1.0}; 
	float diffuse21[] = {1.0, 1.0, 1.0, 1.0}; 
	float position20[] = {1.0, 10.0, 1.0, 0.0}; 
	float position21[] = {50.0, 0.0, 0.0, 0.0}; /* To our right. */
	//float lmodel_ambient[] = {0.3, 0.3, 0.3, 1.0}; /* Set the background ambient lighting. */

	glFrontFace(GL_CCW); /* Front faces defined using a counterclockwise rotation. */
	glDepthFunc(GL_LEQUAL); /* Por defeito e GL_LESS */
	glEnable(GL_DEPTH_TEST); /* Use a depth (z) buffer to draw only visible objects. */
	//glEnable(GL_CULL_FACE); /* Use back face culling to improve speed. */
	//glCullFace(GL_BACK); /* Cull only back faces. */
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  // define luz ambiente
	processIllumination();
	//glEnable(GL_TEXTURE_2D);				//
	pix[0].makeCheckBoard();				// cria texturas
	pix[0].setTexture(2001);
	pix[1].readBMPFile("wood2.bmp");	//pix[1].readBMPFile("textura.bmp");
	pix[1].setTexture(2002);
	pix[2].readBMPFile("0.bmp");
	pix[2].setTexture(2003);
	pix[3].readBMPFile("1.bmp");
	pix[3].setTexture(2004);
	pix[4].readBMPFile("2.bmp");
	pix[4].setTexture(2005);
	pix[5].readBMPFile("3.bmp");
	pix[5].setTexture(2006);
	pix[6].readBMPFile("4.bmp");
	pix[6].setTexture(2007);
	pix[7].readBMPFile("5.bmp");
	pix[7].setTexture(2008);
	pix[8].readBMPFile("6.bmp");
	pix[8].setTexture(2009);
	pix[9].readBMPFile("7.bmp");
	pix[9].setTexture(2010);
	pix[10].readBMPFile("8.bmp");
	pix[10].setTexture(2011);
	pix[11].readBMPFile("9.bmp");
	pix[11].setTexture(2011);
	pix[12].readBMPFile("score.bmp");
	pix[12].setTexture(2012);
	pix[13].readBMPFile("wb.bmp");
	pix[13].setTexture(2013);
	pix[14].readBMPFile("board.bmp");
	pix[14].setTexture(2014);

	// por defeito a cor e de fundo e o preto
	//glClearColor(1.0,1.0,1.0,1.0);

	/* Set the light properties */
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient20);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse20);
	glLightfv(GL_LIGHT0, GL_POSITION, position20);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient20);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse21);
	glLightfv(GL_LIGHT1, GL_POSITION, position21);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_LIGHT1);
	/* Which shade model to use: GL_FLAT / GL_SMOOTH. */
	glShadeModel(GL_SMOOTH);


	glQ = gluNewQuadric();
	gluQuadricOrientation(glQ, GLU_OUTSIDE);

	// display list para a mesa
	glNewList(mesaList, GL_COMPILE);
		glPushMatrix();
		paralelo(10.0,1.0,10.0);
		glPopMatrix();
	glEndList();


	vector<int> inner (col_linhas,0);
	int i=0;
	for(i=0;i<col_linhas;i++){
		tab.push_back(inner);
	}
	
}

void close_game()
{
	executeOp(-1);
	//glutDestroyWindow(main_window);
	exit(0);
}

int main(int argc, char* argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize (DIMX, DIMY);
   glutInitWindowPosition (INITIALPOS_X, INITIALPOS_Y);
   main_window = glutCreateWindow (argv[0]);
   cout<<"Loading..."<<endl;
  
		scene1 = new SceneLoader("dsx/bus.dsx");
		scene1->setNtheme(1);
		if(scene1->loadScene2("dsx/bus.dsx")==-1){
				cout<<"Erro encontrado"<<endl;
				short dummy;
				cin>>dummy;
				return 0;
		}
				scene2 = new SceneLoader("dsx/cafe.dsx");
				scene2->setNtheme(2);
		if(scene2->loadScene2("dsx/cafe.dsx")==-1){
				cout<<"Erro encontrado"<<endl;
				short dummy;
				cin>>dummy;
				return 0;
		}

				scene3 = new SceneLoader("dsx/cinema.dsx");
				scene3->setNtheme(3);
		if(scene3->loadScene2("dsx/cinema.dsx")==-1){
				cout<<"Erro encontrado"<<endl;
				short dummy;
				cin>>dummy;
				return 0;
		}

		scene=scene1;
		oldtheme=0;

   glutDisplayFunc(display);
   GLUI_Master.set_glutReshapeFunc(reshape);
   GLUI_Master.set_glutKeyboardFunc (keyboard);
   GLUI_Master.set_glutMouseFunc(processMouse);
   glutMotionFunc(processMouseMoved);
   glutPassiveMotionFunc(processPassiveMouseMoved);   
   GLUI_Master.set_glutSpecialFunc( NULL );


	/*** Create the bottom subwindow ***/
	glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
														/*	GLUI_SUBWINDOW_RIGHT
															GLUI_SUBWINDOW_LEFT
															GLUI_SUBWINDOW_TOP
															GLUI_SUBWINDOW_BOTTOM */
	glui2->set_main_gfx_window( main_window );

	GLUI_Rotation *view_rot = glui2->add_rotation( "Rotacao", view_rotate );
	view_rot->set_spin( 1.0 );
	glui2->add_column( false );

	GLUI_Translation *trans_z = 
	glui2->add_translation( "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
	trans_z->set_speed( .10 );

	glui2->add_column( false );
	GLUI_Checkbox *check = glui2->add_checkbox("Wireframe", &wireframe );


	 	glui4 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM);
	glui4->set_main_gfx_window( main_window );

		glui4->add_column( false );
	GLUI_Button *u_button = glui4->add_button("Undo",0,(GLUI_Update_CB)undo );

	glui4->add_column( false );
	GLUI_Button *r_button = glui4->add_button("Replay",0,(GLUI_Update_CB)secondReplay );

	glui4->add_column( false );
	GLUI_Button *n_button = glui4->add_button("New game",0,(GLUI_Update_CB)new_game );	

	glui4->add_column( false );
	c_panel=glui4->add_panel("Clock - Segundos Passados");
	clockt=glui4->add_edittext_to_panel(c_panel,"");
	c_panel->disable();
	glui4->disable();

	 	GLUI_Listbox *themes=glui2->add_listbox("Temas",&temas,0,(GLUI_Update_CB)changet);
		themes->add_item(0,"Estrada");
		themes->add_item(1,"Cafe");
		themes->add_item(2,"Cinema");
	
	lightsbar();
   
	glui2->add_column( true );
	GLUI_Checkbox *change_camera = glui2->add_checkbox("Change Camera", &ccamera );
	GLUI_Button *quit = glui2->add_button("Quit Game",0,(GLUI_Update_CB)close_game );

	GLUI_Master.set_glutIdleFunc( myGlutIdle );

	executeOp(1);
	inicializacao();
	
	// numero de objectos para picking
	pk = new picking(5);
	selected=false;
	cout<<"Finished Loading!"<<endl;
	glutMainLoop();

	delete pk;

	return 0;
}
