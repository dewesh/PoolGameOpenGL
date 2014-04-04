/*
NAME: DEWESH AGRAWAL
ROLL: 10CS30018
*/
//**************************************************************************
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <ctime>
#include <string.h>
#include <fstream>
#include <GL/glut.h> 
//**************************************************************************
using namespace std;
//**************************************************************************
#define two(X) (1<<(X))
#define twoL(X) (((int64)(1))<<(X))
#define contain(S,X) (((S)&two(X))!=0)
#define containL(S,X) (((S)&twoL(X))!=0)
const double pi=acos(-1.0);
const double eps=1e-11;
template<class T> inline void checkmin(T &a,T b){if(b<a) a=b;}
template<class T> inline void checkmax(T &a,T b){if(b>a) a=b;}
template<class T> inline T sqr(T x){return x*x;}
typedef pair<int,int> ipair;
typedef long long int lli;
#define SIZE(A) ((int)A.size())
#define LENGTH(A) ((int)A.length())
#define MP(A,B) make_pair(A,B)
#define PB(X) push_back(X)
double MAG = 5;
//************************************************************************** Debug
#define _DEBUG
//******************************** point class *****************************
class point
  {
  public:
  	double x,y,z;
  	point(int, int , int);
  	point();
  };  
  point::point(int a,int b,int c){
  	x =a; y =b; z =c;
  }
  point::point(){
  	x =0; y =0; z =0;
  }
//**************************************************************
class face {
 public:
 	face();
 	vector<int> points;
	double R,G,B;
	bool isColored;
 }; 
 face::face(){
 	R = 0;
 	G = 0;
 	B = 0;
 	isColored = false;
 }
 //**************************************************************

//**************************************************************
bool checkColor(face f, point p){
	if(f.R == p.x && f.G == p.y && f.B == p.z ){
		return true;
	}
	else{
		return false;
	}
}
//**************************** class OBJ ********************************** 
class OBJ{
public:
	vector<point> vertices;
	vector<face> faces;
	vector< point > availableColors;
	OBJ(char* );
	void printObj();
};
//**************************************************************
OBJ::OBJ(char* ObjFileName){
	int faceNum=0,vertexNum=0,edgeNum=0;
	vertices.clear();
	faces.clear();

	int a,b,c;
	char op[100];
	size_t startpos;
	string line,line1;
	point tempPoint; 
	face tempFace; 
	string materialName;
	point materialColor;
	char mtlFileName[30];
	map<string, point> material;

	cout << "lol";
	ifstream infile;
    infile.open(ObjFileName);
	
	while(getline(infile,line)){
		// removing the leading white spaces
		startpos = line.find_first_not_of(" \t");
		if( string::npos != startpos ){
		    line = line.substr( startpos );
		}tempFace.points.push_back(a-1);
		// Parsing the line to get the data
		istringstream iStrStream(line);
		iStrStream >> op;

		if (strcmp(op,"mtllib")==0)	// material population
		{
			iStrStream >> mtlFileName;
			ifstream mtlFile;
    		mtlFile.open(mtlFileName);
    		while(getline(mtlFile,line1)){
    			istringstream iStrStream1(line1);
    			iStrStream1 >> op;
    			if(strcmp(op,"newmtl") == 0){
    				iStrStream1 >> materialName;
    			}
    			if(strcmp(op,"ka") == 0){
    				iStrStream1 >> materialColor.x >> materialColor.y >> materialColor.z; 
    				material[materialName] = materialColor;
    			}
    		}
		}
		if (strcmp(op,"usemtl") == 0){
			iStrStream >> materialName;
		}
		if (op[0] == 'v'){
			iStrStream >> tempPoint.x >> tempPoint.y >> tempPoint.z ;
			tempPoint.x = tempPoint.x * MAG;
			tempPoint.y = tempPoint.y * MAG;
			tempPoint.z = tempPoint.z * MAG; 
			vertices.push_back(tempPoint);
			vertexNum++;
		}
		if (op[0] == 'f'){
			iStrStream >> a >> b >> c ;
			tempFace.points.clear();
			tempFace.points.push_back(a-1);
			tempFace.points.push_back(b-1);
			tempFace.points.push_back(c-1);
			materialColor = material[materialName];
			tempFace.R = materialColor.x;
			tempFace.G = materialColor.y;
			tempFace.B = materialColor.z;
			faces.push_back(tempFace);
		}
	}
}
//**************************************************************
void OBJ::printObj(){
	/*cout << " # printing the vertices data " << endl;
	for (std::vector<point>::iterator i = vertices.begin(); i != vertices.end(); ++i){
		cout << i->x << "  " << i->y << "  " << i->z << endl; 
	}*/
	cout << " # printing the faces data " << endl;
	for (std::vector<face>::iterator i = faces.begin(); i != faces.end(); ++i){
		cout << i->R << " : ";
		cout << i->G << " : ";
		cout << i->B << endl; 
	}
}
OBJ *obj;
//**************************************************************************
void renderScene(void) {
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK,GL_FILL);
	//glEnable(GL_CULL_FACE);
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	point p;
	glPushMatrix();
	for (std::vector<face>::iterator i = obj->faces.begin(); i != obj->faces.end(); ++i)
	{
		glColor3f(i->R,i->G,i->B);
		
		glBegin(GL_TRIANGLES);
		p = obj->vertices[i->points[0]];
		glVertex3f(p.x, p.y, p.z);
		p = obj->vertices[i->points[1]];
		glVertex3f(p.x, p.y, p.z);
		p = obj->vertices[i->points[2]];
		glVertex3f(p.x, p.y, p.z);
		
		glEnd();
	}
	
	glPopMatrix();
	glFlush();
	

}
//**************************************************************

//**************************************************************
int main(int argc, char** argv){
	//--------------------- input ---------------------
	#ifdef _DEBUG
	//freopen("apple.obj","r",stdin);//freopen("output.txt","w",stdout);
	//freopen("error.txt","r",stderr);
	#endif
	//--------------------- Delare Variables ----------
	//--------------------- Process -------------------
	char ObjFileName[100];
	printf("enter the name of the file :: eg. apple-colored.obj\n");
	cin >> ObjFileName;
	printf("Enter the Magnification value\n");
	cin >> MAG;
	obj = new OBJ(ObjFileName);
	glutInit(&argc, argv);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("GLUT");
	glutDisplayFunc(renderScene);
	glutMainLoop();
	//--------------------- Result Display ------------
	return 0;
}



