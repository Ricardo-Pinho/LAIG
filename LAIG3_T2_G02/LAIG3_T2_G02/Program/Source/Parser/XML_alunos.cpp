// G1_Ilum.cpp : Defines the entry point for the console application.
//

#include "SceneLoader.h"
#include "Graph.h"


#include <GL\glui.h>
#include <math.h>
#include <iostream>
#include <sstream>

using namespace std;

// dimensoes e localizacao da janela
#define DIMX 500
#define DIMY 500
#define INITIALPOS_X 200
#define INITIALPOS_Y 200

int light_number=0;

float xy_aspect;

// matriz de transf. geometrica utilizada pelo botao esferico
float view_rotate[16] =	{ 1,0,0,0,
						  0,1,0,0,
						  0,0,1,0,
						  0,0,0,1 };

// vector de posicao utilizado pelo botao de afastamento
float obj_pos[] = { 0.0, 0.0, 0.0 };

// declarações para os tres eixos (cilindros)
double axis_radius_begin =  0.2;
double axis_radius_end   =  0.0;
double axis_lenght       = 16.0;
int axis_nslices = 8;
int axis_nstacks = 1;

// declaracoes para a esfera de origem de coordenadas
double orig_radius = 0.5;
int orig_slices = 8;
int orig_stacks =16;
	
// declaracoes para o plano e caixa
float mat1_shininess[] = {128.0}; 

//float mat1_specular[] = {1.0,1.0,1.0};	/* specular reflection. */
float mat1_specular[] = {0.4, 0.4, 0.4, 1.0};	/* specular reflection. */
//float mat1_diffuse[] =  {0.0, 0.0, 0.0, 1.0};	/* diffuse reflection. */
float mat1_diffuse[] =  {0.7, 0.7, 0.7, 1.0};	/* diffuse reflection. */
float mat1_ambient[] =  {0.7, 0.7, 0.7, 1.0};	/* ambient reflection. */
double dimx= 6.0;
double dimy= 2.0;
double dimz=10.0;

float light0_kc=0.0;
float light0_kl=0.0;
float light0_kq=0.1;

// declarações para as stripes que formam o plano
double i,j;
double di, limi=dimx, divisoes_i = 60;	//60
double dj, limj=dimz, divisoes_j = 100;	//100

// declarações para a fonte de luz LIGHT0;
float light0_position[]  = {0.0, 3.0, 4.0, 1.0}; // nao necessaria...
float light0_ambient[] =   {0.5, 0.5, 0.5, 1.0}; // sem componente ambiente
//float light0_diffuse[] =   {1.0, 1.0, 0.0, 0.0};
float light0_diffuse[] =   {0.5, 0.5, 0.5, 1.0};
//float light0_specular[] =  {0.0, 0.0, 0.0, 0.0};
float light0_specular[] =  {0.7, 0.7, 0.7, 1.0};
double light0x = 0;
double light0y = 0;
double light0z = -1;
double symb_light0_radius = 0.2;	// esfera que
int symb_light0_slices = 8;			// simboliza a
int symb_light0_stacks =16;			// fonte de luz light0

// fonte (global) de luz ambiente 
//float light_ambient[] = {0.0, 0.0, 0.0, 1.0}; /* Set the background ambient lighting. */
float light_ambient[] = {0.6, 0.6, 0.6, 1.0}; /* Set the background ambient lighting. */

// variaveis para a janela
int main_window;
GLUI  *glui2;

SceneLoader *scene;
int wireframe =false;


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

 void draw_axis(GLUquadric* glQ)
 {
	 glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	 //desenhar a esfera no centro
	glColor3f(1.0,1.0,1.0);		
	gluSphere(glQ, orig_radius, orig_slices, orig_stacks);

//  aumentar e diminuir a atenuação

	// esfera representativa da origem das coordenadas
	// falta declarar a cor
	// desenhar o objecto

	// cilindro representativo do eixo X		// vermelho

	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glRotated(90.0, 0.0,1.0,0.0 );
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             scene->scene_length_axis, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();

	// cilindro representativo do eixo Y	// verde
	glPushMatrix();
	glColor3f(0.0,1.0,0.0);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             scene->scene_length_axis, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();
	// cilindro representativo do eixo Z	// azul
	glPushMatrix();
	glColor3f(0.0,0.0,1.0);	
	// nao necessita rotacao... glRotated(...);
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             scene->scene_length_axis, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat1_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat1_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat1_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat1_ambient);
	// inibicao de atribuicao directa de cores
 }

void display(void)
{
	// ****** declaracoes internas 'a funcao display() ******

	GLUquadric* glQ;	// nec. p/ criar sup. quadraticas (cilindros, esferas...)

	// ****** fim de todas as declaracoes da funcao display() ******

	glQ = gluNewQuadric();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	// inicializacoes da matriz de visualizacao
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 500.0 );

	//inicializacoes da matriz de transformacoes geometricas
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	// afasta a cena de 25 unidades mais a distância...
	glTranslated(0.0,0.0,-25.0);
	// ...decorrente da utilizacao do botao de afastamento
    glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] );    

	// roda a cena para ficar em perspectiva
	glRotated(20.0, 1.0,0.0,0.0 );		// 20 graus em torno de X
	glRotated(-45.0, 0.0,1.0,0.0 );	//-45 graus em torno de Y
	
	// aplica efeito do botao de rotacao
	glMultMatrixf( view_rotate );

	// permissao de atribuicao directa de cores
	// para objectos ue nao tem material atribuido
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glPushMatrix();
	//glTranslated(light0x,light0y,light0z);
	//gluSphere(glQ, symb_light0_radius, symb_light0_slices, symb_light0_stacks);
    //glPopMatrix();
	//scene->map_vistas[scene->current_View]->setView();
	processLights();
	gluQuadricOrientation(glQ, GLU_OUTSIDE);

	//draw_axis(glQ);
	// define caracteristicas de cor do material do plano e da caixa

	//desenhaPolygonos();
	//pair<string,No*> root=scene->getIndependendcomponent();
	//scene->printGraph(glQ,root,1,1);
						/*	glPushMatrix();
					glTranslated(0,20,0);
					gluSphere(glQ, 1, symb_light0_slices, symb_light0_stacks);
					glPopMatrix();*/
	if(wireframe){
	
		glPolygonMode(GL_FRONT, GL_LINE );	// preence a face da frente dos poligonos
	}
	else{
		glPolygonMode(GL_FRONT, GL_FILL);
	}
	No* root = scene->map_componentes[scene->root_id];
	// cout<<"I GOT EVEN HERE1"<<endl;
	// processComponent(root);
	 root->draw();
	
	 //lights
	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
   
	glFlush();
}


/* Mouse handling */
void processMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{	 
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{	
	}
	
	glutPostRedisplay();
	
}

void processMouseMoved(int x, int y)
{
	
	// pedido de refrescamento da janela
	glutPostRedisplay();				

}

void processPassiveMouseMoved(int x, int y)
{

	// pedido de refrescamento da janela
	glutPostRedisplay();				
}

void reshape(int w, int h)
{
	int tx, ty, tw, th;

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
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

	glFrontFace(GL_CCW);		/* Front faces defined using a counterclockwise rotation. */
	glDepthFunc(GL_LEQUAL);		/* Por defeito e GL_LESS */
	glEnable(GL_DEPTH_TEST);	/* Use a depth (z) buffer to draw only visible objects. */
	glEnable(GL_CULL_FACE);		/* Use face culling to improve speed. */
	glCullFace(GL_BACK);		/* Cull only back faces. */

	// por defeito a cor e de fundo e o preto
	//glClearColor(1.0,1.0,1.0,1.0);    // cor de fundo a branco


	processIllumination();

		/*glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);  // define luz ambiente
	glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, 1);*/
	
	// por defeito a cor de fundo e o preto
	// glClearColor(1.0,1.0,1.0,1.0);    // cor de fundo a branco


	// declaracoes para a fonte luz GL_LIGHT0
	/*glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);*/

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);				// GL_FLAT / GL_SMOOTH

	glPolygonMode(GL_FRONT, GL_FILL);	// preence a face da frente dos poligonos
	//glPolygonMode(GL_FRONT, GL_LINE);	// desenha arestas dos poligonos
	scene->current_View=scene->default_View;
	
}


void newView(){
	map<string, View*>::iterator it;
	it=scene->map_vistas.find(scene->current_View);
	it++;
	if(it==scene->map_vistas.end())
		it=scene->map_vistas.begin();

	scene->current_View=it->second->id;
}
int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (DIMX, DIMY);
	glutInitWindowPosition (INITIALPOS_X, INITIALPOS_Y);
	main_window = glutCreateWindow (argv[0]);
 
	cout<<"Indique o nome do ficheiro na pasta dsx(Default é:bus.dsx):"<<endl;
	string filename;
	cin>>filename;
	char* fn;
	fn = new char[16];
	strcpy (fn, "dsx/");
	strcat (fn, filename.c_str());
	if(filename==""){
		scene = new SceneLoader("dsx/bus.dsx");
		
		if(scene->loadScene2("dsx/bus.dsx")==-1){
				cout<<"Erro encontrado"<<endl;
				short dummy;
				cin>>dummy;
				return 0;
		}
	}
	else{
		scene = new SceneLoader(fn);

		if(scene->loadScene2(fn)==-1){
				cout<<"Erro encontrado"<<endl;
				short dummy;
				cin>>dummy;
				return 0;
		}
	}
   glutDisplayFunc(display);
   GLUI_Master.set_glutReshapeFunc(reshape);
   GLUI_Master.set_glutKeyboardFunc (keyboard);
   GLUI_Master.set_glutMouseFunc(processMouse);
   glutMotionFunc(processMouseMoved);
   glutPassiveMotionFunc(processPassiveMouseMoved);   
   GLUI_Master.set_glutSpecialFunc( NULL );
   

	/*** Create the bottom subwindow ***/
	glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
	glui2->set_main_gfx_window( main_window );
	
	GLUI_Rotation *view_rot = glui2->add_rotation( "Rotacao", view_rotate );
	view_rot->set_spin( 1.0 );
	glui2->add_column( false );
	int counter=0;
	map<string, Light*>::iterator it;

	GLUI_Translation *trans_z = 
	glui2->add_translation( "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
	trans_z->set_speed( .10 );

	glui2->add_column( false );
	GLUI_Checkbox *check = glui2->add_checkbox("Wireframe", &wireframe );
	GLUI_Button *button = glui2->add_button("Change",0,(GLUI_Update_CB)newView );
	 counter=0;

	 for(it= scene->map_luzes.begin();it!=scene->map_luzes.end();it++){
		stringstream ss;//create a stringstream
		ss << "Light number-"<<counter;
		string name=ss.str();
		char button[16];
		button[0]=0;
		strcat (button,name.c_str());
		GLUI_Checkbox *check = glui2->add_checkbox(button, &scene->map_luzes[(*it).first]->enabled );
		counter++;
	 }
	 counter=0;
	/* We register the idle callback with GLUI, not with GLUT */
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
   
	inicializacao();
   
	glutMainLoop();

	return 0;
}
