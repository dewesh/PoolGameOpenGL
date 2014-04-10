#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "World.h"

#define KEY_ESCAPE 27
#define left_arr 97
#define right_arr 100
#define up_arr 119
#define down_arr 115
#define space 32


using namespace std;

typedef struct {
    int width;
	int height;
	char* title;
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

using namespace std;
 
//**************************************************************************

glutWindow win;
int currMousePosX,currMousePosY,camPosX=0,camPosY=50;
float Rotation;
World world;
point velocity,pos,gravity,pos_cube,vel_cube;
double xmin=-100,xmax=100,zmin=-100,zmax=100;
int _floor=-10,rot=0;

//**************************************************************
void checkCollisionWithTable(Ball *b){
	if (b->pos.x < xmin+b->radius ){
		b->pos.x = xmin+b->radius;
		b->velocity.x = -(b->velocity.x);
		b -> previousCollison = -1;
	}
	if(b->pos.x > xmax-b->radius){
		b->pos.x = xmax-b->radius;
		b->velocity.x = -(b->velocity.x);
		b -> previousCollison = -1;
	}
	if (b->pos.z < zmin+b->radius ){
		b->pos.z = zmin+b->radius;
		b->velocity.z = -(b->velocity.z);
		b -> previousCollison = -1;
	}
	if(b->pos.z > zmax-b->radius){
		b->pos.z = zmax-b->radius;
		b->velocity.z = -(b->velocity.z);
		b -> previousCollison = -1;
	}
}
//**************************************************************
void drawRect(double b,double l){
	glBegin(GL_QUADS);
	glVertex3f(-l/2,0,b/2);
	glVertex3f(-l/2,0,-b/2);
	glVertex3f(l/2,0,-b/2);
	glVertex3f(l/2,0,b/2);
	glEnd();
}
//**************************************************************
void drawTable(Table *t){
	glPushMatrix();										  // Push the current matrix stack
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		for (int i = xmin; i <= xmax; i += 5) {
			glVertex3f(i, 0, zmax); glVertex3f(i, 0, zmin);
		}
		for (int i = zmin; i <= zmax; i += 5) {
			glVertex3f(xmax, 0, i); glVertex3f(xmin, 0, i);
		}
		glEnd();
	glPopMatrix();  // Pop the current matrix stack	

	glPushMatrix();				//surface						  
		glColor3f(0.1,1,0);
        drawRect(t->len+2*t->thickness,t->bre+2*t->thickness);
	glPopMatrix();				

		glColor3f(0.5,0.2,0.2);
	//**************************************************************
		glPushMatrix();				//sidebar	
			glTranslatef(t->bre/2,t->height/2,0);				  
			glRotatef(-90,0,0,1);
	        drawRect(t->len+2*t->thickness,t->height);
		glPopMatrix();				
		glPushMatrix();				// sidebar	
			glTranslatef(t->bre/2+t->thickness,t->height/2,0);						  
			glRotatef(-90,0,0,1);
	        drawRect(t->len+2*t->thickness,t->height);
		glPopMatrix();
		glPushMatrix();				// sidebar
			glTranslatef(-t->bre/2-t->thickness,t->height/2,0);	
			glRotatef(+90,0,0,1);
	        drawRect(t->len+2*t->thickness,t->height);
		glPopMatrix();
		glPushMatrix();				// sidebar	
			glTranslatef(-t->bre/2,t->height/2,0);							  
			glRotatef(+90,0,0,1);
	        drawRect(t->len+2*t->thickness,t->height);
		glPopMatrix();
	//**************************************************************
		glPushMatrix();				//sidebar	
			glTranslatef(0,t->height/2,t->len/2);				  
			glRotatef(-90,1,0,0);
			glRotatef(-90,0,1,0);
	        drawRect(t->bre+2*t->thickness,t->height);
		glPopMatrix();				
		glPushMatrix();				// sidebar	
			glTranslatef(0,t->height/2,t->len/2+t->thickness);
			glRotatef(-90,1,0,0);						  
			glRotatef(-90,0,1,0);
	        drawRect(t->bre+2*t->thickness,t->height);
		glPopMatrix();
		glPushMatrix();				// sidebar
			glTranslatef(0,t->height/2,-t->len/2-t->thickness);	
			glRotatef(-90,1,0,0);
			glRotatef(+90,0,1,0);
	        drawRect(t->bre+2*t->thickness,t->height);
		glPopMatrix();
		glPushMatrix();				// sidebar	
			glTranslatef(0,t->height/2,-t->len/2);	
			glRotatef(-90,1,0,0);						  
			glRotatef(+90,0,1,0);
	        drawRect(t->bre+2*t->thickness,t->height);
		glPopMatrix();
	//**************************************************************
		glPushMatrix();				//topBar	
			glTranslatef(0,t->height,t->len/2+t->thickness/2);				  
			glRotatef(90,0,1,0);
	        drawRect(t->bre,t->thickness);
		glPopMatrix();	
		glPushMatrix();				// topBar	
			glTranslatef(0,t->height,-t->len/2-t->thickness/2);	
			glRotatef(90,0,1,0);
	        drawRect(t->bre,t->thickness);
		glPopMatrix();			
			glPushMatrix();				// topBar	
			glTranslatef(-t->bre/2-t->thickness/2,t->height,0);							  
	        drawRect(t->len+2*t->thickness,t->thickness);
		glPopMatrix();
		glPushMatrix();				//topBar	
			glTranslatef(t->bre/2+t->thickness/2,t->height,0);				  
	        drawRect(t->len+2*t->thickness,t->thickness);
		glPopMatrix();
	//**************************************************************
}
//**************************************************************
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	// GLfloat position1[] = { 00.0, 100.0, 00.0, 1.0 };
	
	//cam position update
	gluLookAt( world.camera->cameraFrom.x,world.camera->cameraFrom.y,world.camera->cameraFrom.z, world.camera->cameraTo.x,0,world.camera->cameraTo.z, 0,1,0);	 // Define a viewing transformation
									  // Pop the current matrix stack

	drawTable(world.table);
	//current stick
	glPushMatrix();		
		glColor3f(0,0,1);								  // Push the current matrix stack
		GLUquadricObj *quadratic;
  		quadratic = gluNewQuadric();
  		glTranslatef(world.stick.pos.x,world.qBall.radius,world.stick.pos.z);
  		glRotatef(world.stick.angle,0,1,0);
  		gluCylinder(quadratic,world.qBall.radius*0.2,world.qBall.radius * 0.5,world.qBall.radius*30.0,32,32);
	glPopMatrix();  // Pop the current matrix stack	
	//que ball update
	glPushMatrix();										  // Push the current matrix stack
		glColor3f(1,1,1);
		glTranslatef(world.qBall.pos.x,world.qBall.pos.y,world.qBall.pos.z);  //Multiply the current matrix by a translation matrix
		glutSolidSphere  (world.qBall.radius,50,50); 
	glPopMatrix();  // Pop the current matrix stack
	
	//general ball update
	for (int i = 0; i < 15; ++i)
	{
		glPushMatrix();										  // Push the current matrix stack
			glColor3f((double)1/i+1,(double)1/i+1,0);
			glTranslatef(world.ball[i].pos.x,world.ball[i].pos.y,world.ball[i].pos.z);  //Multiply the current matrix by a translation matrix
			glutSolidSphere  (world.ball[i].radius,50,50); 
		glPopMatrix();  // Pop the current matrix stack
	}
	//holes
	for (int i = 0; i < 4; ++i)
	{
		glPushMatrix();										  // Push the current matrix stack
			glColor3f(0,0,0);
			glTranslatef(world.holes[i].pos.x,world.holes[i].pos.y,world.holes[i].pos.z);  //Multiply the current matrix by a translation matrix
			glutSolidSphere  (world.holes[i].radius,50,50); 
		glPopMatrix();  // Pop the current matrix stack
	}
	world.update();
	for (int i = 0; i < 15; ++i)
	{
		checkCollisionWithTable(&world.ball[i]);
	}
	checkCollisionWithTable(&world.qBall);
	glutSwapBuffers();
}


void initialize () 
{	
    glMatrixMode(GL_PROJECTION);												// select projection matrix
    glViewport(0, 0, win.width, win.height);									// set the viewport
    glMatrixMode(GL_PROJECTION);												// set matrix mode
    glLoadIdentity();															// reset projection matrix
    GLfloat aspect = (GLfloat) win.width / win.height;
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);		// set up a perspective projection matrix
    glMatrixMode(GL_MODELVIEW);													// specify which matrix is the current matrix
    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );														// specify the clear value for the depth buffer
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );						// specify implementation-specific hints

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
	glClearColor(0.0, 0.0, 1.0, 1.0);

}
//**************************************************************
void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )		
{ 
  switch ( key ) 
  {
    case left_arr:
		//camPosX--;
    	if(world._STATE == POSITIONSTICK)
		world.stick.update(-1,0);
    	//rot -=1;
    	break;
    case right_arr:
		// camPosX ++;
    	//rot +=1;
    	if(world._STATE == POSITIONSTICK)
    	world.stick.update(1,0);
    	break;
    case up_arr:
		// camPosY--;
    	if(world._STATE == POSITIONSTICK)
		world.stick.update(0,1);	
    	//velocity.y +=0.5;
    	break;
    case down_arr:
		// camPosY++;
    	if(world._STATE == POSITIONSTICK)
		world.stick.update(0,-1);	
    	//velocity.y -=0.5;
	    break;
    case space:
		//velocity.y = 3;
		break;
    case KEY_ESCAPE:        
    	exit ( 0 );   
	    break;      

    default:      
	    break;
  }
}
//**************************************************************
void mouse ( int key1, int key2, int mousePositionX, int mousePositionY )		
{ 

	if(key1 == 0 && key2 == 1 && world._STATE == POSITIONSTICK){
		//world.reset();
		world._STATE = TAKESHOT;		
	}
	cout << key2;
}
//**************************************************************
void myMouseFunc(int x, int y){
	//cout << x << ":" << y << endl;
	currMousePosX = x;
	currMousePosY = y;
}
//**************************************************************
int main(int argc, char **argv) 
{
	// set window values
	win.width = 640;
	win.height = 480;
	win.title = "OpenGL/GLUT Window.";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;
	xmin = world.table->p2.x;
	xmax = world.table->p1.x;
	zmax = world.table->p1.z;
	zmin = world.table->p3.z;
	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);					// set window size
	glutCreateWindow(win.title);								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc( display );									// register Idle Function
    	glutKeyboardFunc( keyboard );								// register Keyboard Handler
    	glutMouseFunc( mouse ); 
	glutPassiveMotionFunc(myMouseFunc);
	initialize();
	//obj.Load("woman.obj");
	//coloumn.Load("coloumn.obj");

	glutMainLoop();												// run GLUT mainloop
	return 0;
}
