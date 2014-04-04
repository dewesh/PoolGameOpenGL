#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "World.h"
#include "Ball.h"
#include "Stick.h"
#include "Camera.h"

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


 
/*************************************************************************** 
  OBJ Loading 
 ***************************************************************************/
 //**************************************************************

class Model_OBJ
{
  public: 
	Model_OBJ();			
	float* calculateNormal(float* coord1,float* coord2,float* coord3 );
	int Load(char *filename);	// Loads the model
	void Draw();					// Draws the model on the screen
	void Release();				// Release the model

	float* normals;							// Stores the normals
	float* Faces_Triangles;					// Stores the triangles
	float* vertexBuffer;					// Stores the points which make the object
	long TotalConnectedPoints;				// Stores the total number of connected verteces
	long TotalConnectedTriangles;			// Stores the total number of connected triangles
 
};
 
 //**************************************************************
#define POINTS_PER_VERTEX 3
#define TOTAL_FLOATS_IN_TRIANGLE 9
using namespace std;
 
Model_OBJ::Model_OBJ()
{
	this->TotalConnectedTriangles = 0; 
	this->TotalConnectedPoints = 0;
}
 
float* Model_OBJ::calculateNormal( float *coord1, float *coord2, float *coord3 )
{
	/* calculate Vector1 and Vector2 */
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

	/* cross product */
	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	/* normalization factor */
	val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );

	float norm[3];
	norm[0] = vr[0]/val;
	norm[1] = vr[1]/val;
	norm[2] = vr[2]/val;


	return norm;
}
 
 //**************************************************************
int Model_OBJ::Load(char* filename)
{
	string line;
	ifstream objFile (filename);	
	if (objFile.is_open())													// If obj file is open, continue
	{
		objFile.seekg (0, ios::end);										// Go to end of the file, 
		long fileSize = objFile.tellg();									// get file size
		objFile.seekg (0, ios::beg);										// we'll use this to register memory for our 3d model
 
		vertexBuffer = (float*) malloc (fileSize);							// Allocate memory for the verteces
		Faces_Triangles = (float*) malloc(fileSize*sizeof(float));			// Allocate memory for the triangles
		normals  = (float*) malloc(fileSize*sizeof(float));					// Allocate memory for the normals
 
		int triangle_index = 0;												// Set triangle index to zero
		int normal_index = 0;												// Set normal index to zero
 
		while (! objFile.eof() )											// Start reading file data
		{		
			getline (objFile,line);											// Get line from file
 
			if (line.c_str()[0] == 'v')										// The first character is a v: on this line is a vertex stored.
			{
				line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
 
				sscanf(line.c_str(),"%f %f %f ",							// Read floats from the line: v X Y Z
					&vertexBuffer[TotalConnectedPoints],
					&vertexBuffer[TotalConnectedPoints+1], 
					&vertexBuffer[TotalConnectedPoints+2]);
 
				TotalConnectedPoints += POINTS_PER_VERTEX;					// Add 3 to the total connected points
			}
			if (line.c_str()[0] == 'f')										// The first character is an 'f': on this line is a point stored
			{
		    	line[0] = ' ';												// Set first character to 0. This will allow us to use sscanf
 
				int vertexNumber[4] = { 0, 0, 0 };
                sscanf(line.c_str(),"%i%i%i",								// Read integers from the line:  f 1 2 3
					&vertexNumber[0],										// First point of our triangle. This is an 
					&vertexNumber[1],										// pointer to our vertexBuffer list
					&vertexNumber[2] );										// each point represents an X,Y,Z.
 
				vertexNumber[0] -= 1;										// OBJ file starts counting from 1
				vertexNumber[1] -= 1;										// OBJ file starts counting from 1
				vertexNumber[2] -= 1;										// OBJ file starts counting from 1
 
 
				/********************************************************************
				 * Create triangles (f 1 2 3) from points: (v X Y Z) (v X Y Z) (v X Y Z). 
				 * The vertexBuffer contains all verteces
				 * The triangles will be created using the verteces we read previously
				 */
 
				int tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)					
				{
					Faces_Triangles[triangle_index + tCounter   ] = vertexBuffer[3*vertexNumber[i] ];
					Faces_Triangles[triangle_index + tCounter +1 ] = vertexBuffer[3*vertexNumber[i]+1 ];
					Faces_Triangles[triangle_index + tCounter +2 ] = vertexBuffer[3*vertexNumber[i]+2 ];
					tCounter += POINTS_PER_VERTEX;
				}
 
				/*********************************************************************
				 * Calculate all normals, used for lighting
				 */ 
				float coord1[3] = { Faces_Triangles[triangle_index], Faces_Triangles[triangle_index+1],Faces_Triangles[triangle_index+2]};
				float coord2[3] = {Faces_Triangles[triangle_index+3],Faces_Triangles[triangle_index+4],Faces_Triangles[triangle_index+5]};
				float coord3[3] = {Faces_Triangles[triangle_index+6],Faces_Triangles[triangle_index+7],Faces_Triangles[triangle_index+8]};
				float *norm = this->calculateNormal( coord1, coord2, coord3 );
 
				tCounter = 0;
				for (int i = 0; i < POINTS_PER_VERTEX; i++)
				{
					normals[normal_index + tCounter ] = norm[0];
					normals[normal_index + tCounter +1] = norm[1];
					normals[normal_index + tCounter +2] = norm[2];
					tCounter += POINTS_PER_VERTEX;
				}
 
				triangle_index += TOTAL_FLOATS_IN_TRIANGLE;
				normal_index += TOTAL_FLOATS_IN_TRIANGLE;
				TotalConnectedTriangles += TOTAL_FLOATS_IN_TRIANGLE;			
			}	
		}
		objFile.close();														// Close OBJ file
	}
	else 
	{
		cout << "Unable to open file";								
	}
	return 0;
}
 //**************************************************************
void Model_OBJ::Release()
{
	free(this->Faces_Triangles);
	free(this->normals);
	free(this->vertexBuffer);
}
 //**************************************************************
void Model_OBJ::Draw()
{
 	glEnableClientState(GL_VERTEX_ARRAY);						// Enable vertex arrays
 	glEnableClientState(GL_NORMAL_ARRAY);						// Enable normal arrays
	glVertexPointer(3,GL_FLOAT,	0,Faces_Triangles);				// Vertex Pointer to triangle array
	glNormalPointer(GL_FLOAT, 0, normals);						// Normal pointer to normal array
	glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles);		// Draw the triangles
	glDisableClientState(GL_VERTEX_ARRAY);						// Disable vertex arrays
	glDisableClientState(GL_NORMAL_ARRAY);						// Disable normal arrays
}
 //**************************************************************************

glutWindow win;
int currMousePosX,currMousePosY,camPosX=0,camPosY=50;
Model_OBJ obj,coloumn;
float Rotation;
World world;
point velocity,pos,gravity,pos_cube,vel_cube;
double xmin=-60,xmax=60,zmin=-60,zmax=60;
int _floor=-10,rot=0;

//**************************************************************
void checkCollisionWithTable(){
	for (int i = 0; i < 15; ++i)
	{
		if (world.ball[i].pos.x < xmin ){
			world.ball[i].pos.x = xmin;
			world.ball[i].velocity.x = -world.ball[i].velocity.x/1.02;
		}
		if(world.ball[i].pos.x > xmax){
			world.ball[i].pos.x = xmax;
			world.ball[i].velocity.x = -world.ball[i].velocity.x/1.02;
		}
		if (world.ball[i].pos.z < zmin ){
			world.ball[i].pos.z = zmin;
			world.ball[i].velocity.z = -world.ball[i].velocity.z/1.02;
		}
		if(world.ball[i].pos.z > zmax){
			world.ball[i].pos.z = zmax;
			world.ball[i].velocity.z = -world.ball[i].velocity.z/1.02;
		}
	}
	if (world.qBall.pos.x < xmin ){
		world.qBall.pos.x = xmin;
		world.qBall.velocity.x = -world.qBall.velocity.x;
	}
	if(world.qBall.pos.x > xmax){
		world.qBall.pos.x = xmax;
		world.qBall.velocity.x = -world.qBall.velocity.x;
	}
	if (world.qBall.pos.z < zmin ){
		world.qBall.pos.z = zmin;
		world.qBall.velocity.z = -world.qBall.velocity.z;
	}
	if(world.qBall.pos.z > zmax){
		world.qBall.pos.z = zmax;
		world.qBall.velocity.z = -world.qBall.velocity.z;
	}
	
}
//**************************************************************
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	//cam position update
	gluLookAt( camPosX,60,camPosY, camPosX+(currMousePosX-win.width/2)/2,(-currMousePosY+win.height/2)/2,camPosY-50, 0,1,0);	 // Define a viewing transformation
									  // Pop the current matrix stack
	//general ball update
	
	glPushMatrix();										  // Push the current matrix stack
		glColor3f(1,0,0);
		glTranslatef(0,-60,0);  //Multiply the current matrix by a translation matrix
		glutSolidCube  (120); 
	glPopMatrix();  // Pop the current matrix stack	
	//que ball update
	glPushMatrix();										  // Push the current matrix stack
		glColor3f(1,1,1);
		glTranslatef(world.qBall.pos.x,world.qBall.pos.y,world.qBall.pos.z);  //Multiply the current matrix by a translation matrix
		glutSolidSphere  (world.qBall.radius,50,50); 
	glPopMatrix();  // Pop the current matrix stack
	for (int i = 0; i < 15; ++i)
	{
		glPushMatrix();										  // Push the current matrix stack
			glColor3f((double)1/i+1,(double)1/i+1,0);
			glTranslatef(world.ball[i].pos.x,world.ball[i].pos.y,world.ball[i].pos.z);  //Multiply the current matrix by a translation matrix
			glutSolidSphere  (world.ball[i].radius,50,50); 
		glPopMatrix();  // Pop the current matrix stack
	}
	world.update();
	checkCollisionWithTable();

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


void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )		
{ 
  switch ( key ) 
  {
    case left_arr:
camPosX--;
    	//rot -=1;
    	break;
    case right_arr:
camPosX ++;
    	//rot +=1;
    	break;
    case up_arr:
camPosY--;
    	//velocity.y +=0.5;
    	break;
    case down_arr:
camPosY++;
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

void mouse ( int key1, int key2, int mousePositionX, int mousePositionY )		
{ 

	if(key1 == 0){
		world.reset();
		world.qBall.velocity.x =mousePositionX/100;
		world.qBall.velocity.z =mousePositionY/100;
		
	}
	//cout << mousePositionX;
}
void myMouseFunc(int x, int y){
	cout << x << ":" << y << endl;
	currMousePosX = x;
	currMousePosY = y;
}
int main(int argc, char **argv) 
{
	// set window values
	win.width = 640;
	win.height = 480;
	win.title = "OpenGL/GLUT Window.";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

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
