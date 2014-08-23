#pragma once
#include "Light.h"
#include <math.h>

class Spot: public Light{
public:
	float angle;
	float exponent;
	XYZ target;
	Spot(string _id,	bool _enabled,	XYZ _location, 	RGBA _ambient, RGBA _diffuse, RGBA _specular,float _angle,float _exp,XYZ _tar):Light( _id,	 _enabled,	 _location, _ambient, _diffuse, _specular){
		angle=_angle;
		exponent=_exp;
		target=_tar;
		setType("spot");
	}
	Spot(string i):Light(i){setType("spot");}

	void setAngle(float ang){angle=ang;}
	void setExponent(float exp){exponent=exp;}
	void setTarget(XYZ tar){target=tar;}
	
	#define PI 3.14159265
	float* calcDirection(){
			float* result;
			result=(float*)malloc(4*sizeof(float*));

			float deltax=target.x-location.x;
			float deltay=target.y-location.y;
			float deltaz=target.z-location.z;
			/*if(deltax>=deltay&&deltax>=deltaz){
				float vecx=1.0;
				//y e x
				float vecy=deltay/deltax;

				//z e x
				float vecz=deltaz/deltax;

				if(target.x<location.x)
					vecx*=-1.0;
				if(target.y<location.y)
					vecy*=-1.0;
				if(target.z<location.z)
					vecz*=-1.0;

				result[0]=deltax;
				result[1]=deltay;
				result[2]=deltaz;
				return result;
			}
			else if(deltay>=deltax&&deltay>=deltaz){
				float vecy=1.0;
				//x e y
				float vecx=deltax/deltay;
				//z e y
				float vecz=deltaz/deltay;

				if(target.x<location.x)
					vecx*=-1.0;
				if(target.y<location.y)
					vecy*=-1.0;
				if(target.z<location.z)
					vecz*=-1.0;

				result[0]=vecx;
				result[1]=vecy;
				result[2]=vecz;
				return result;

			}
			else if(deltaz>=deltax&&deltaz>=deltay){
				float vecz=1.0;
				//x e z
				float vecx=deltax/deltaz;
			//	float anglex;
			//	anglex = atan (deltax/deltay) * 180 / PI;
				
				//y e z
				float vecy=deltay/deltaz;

				if(target.x<location.x)
					vecx*=-1.0;
				if(target.y<location.y)
					vecy*=-1.0;
				if(target.z<location.z)
					vecz*=-1.0;
				
				result[0]=vecx;
				result[1]=vecy;
				result[2]=vecz;
				return result;
			}
			else{
				cout<<"erro no calculo da direcao"<<endl;
			}*/
			result[0]=deltax;
			result[1]=deltay;
			result[2]=deltaz;
			return result;

	}
	float getExponent(){
		return exponent;
	}
	float getAngle(){
		return angle;
	}
	void setDirection(float *direction)
	{};

	void makeLight(int light_number){
		 float x=location.x;
				 float y=location.y;
				 float z=location.z;
				 //cout<<x<<" "<<y<<" "<<z<<endl;
				float light0_position[]  = {x, y, z, 1}; // nao necessaria...


				 float* light0_ambient=getAmbient();
		

				 
				 float* light0_diffuse=getDiffuse();


				 float* light0_specular= getSpecular();

				float* light0_direction=calcDirection();
				

					// declaracoes para a fonte luz GL_light1
					glLightfv(GL_LIGHT0+light_number, GL_POSITION, light0_position);
					glLightfv(GL_LIGHT0+light_number, GL_AMBIENT, light0_ambient);
					glLightfv(GL_LIGHT0+light_number, GL_DIFFUSE, light0_diffuse);
					glLightfv(GL_LIGHT0+light_number, GL_SPECULAR, light0_specular);

					glLightf(GL_LIGHT0+light_number, GL_SPOT_EXPONENT, getExponent());
					//cout<<light1_position[0]<<" "<<light1_position[1]<<" "<<light1_position[2]<<endl;
					//cout<<light0_position[0]<<" "<<light0_position[1]<<" "<<light0_position[2]<<endl;
					glLightf(GL_LIGHT0+light_number, GL_SPOT_CUTOFF, getAngle());

					glLightfv(GL_LIGHT0+light_number, GL_SPOT_DIRECTION, light0_direction);

					GLUquadric* glQ= gluNewQuadric();
					glEnable(GL_LIGHT0+light_number);
	};
};