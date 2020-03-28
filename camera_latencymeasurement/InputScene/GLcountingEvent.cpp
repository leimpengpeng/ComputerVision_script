#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <math.h>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include "mqtt.h"

using namespace std;

#define PI 3.14159265f
// Global variables
int windowWidth  = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y

// Circle 1 
GLfloat ballRadius = 0.1;   // Radius of the bouncing ball
GLfloat ballX = 0.5f;         // Ball's center (x, y) position
GLfloat ballY = 0.5f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin; // Ball's center (x, y) bounds
GLfloat xSpeed = 0.2f;      // Ball's speed in x and y directions
GLfloat ySpeed = 0.2f;
int refreshMillis = 30;      // Refresh period in milliseconds
 int thour,tmin;
 bool count_start=0;
// Circle 2 
GLfloat ballRadius1 = 0.2f;   // Radius of the bouncing ball
GLfloat ballX1 = -0.5f;         // Ball's center (x, y) position
GLfloat ballY1 = -0.5f;
GLfloat ballXMax1, ballXMin1, ballYMax1, ballYMin1; // Ball's center (x, y) bounds
GLfloat xSpeed1 = 0.5f;      // Ball's speed in x and y directions
GLfloat ySpeed1 = 0.5f;
auto t_start=std::chrono::high_resolution_clock::now();
char title[] = "Bouncing Ball (2D)";  
GLdouble quadLength=0.25;
int count=0;
int count1=0;
bool exceeds =false;
int start=1;
int number=0;
 double Last = 0;
 double Now = 0;
// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
 GLdouble ballXQuadleft, ballXQuadRight, ballYQuadBottom,ballYQuadTop;  


void publish_result(int id, double time){
  MqttWrapper *mqttclient = new MqttWrapper(NULL, "localhost", 1883);
    stringstream str;  
    
    str<< id << " " <<  time;
	mqttclient->publish(NULL, "test", str.str().size(), str.str().c_str());
	
	mqttclient->loop_forever();	
} 
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(1.0, 1.0, 1.0, 1.0); // Set background (clear) color to black

   }

void drawCircle(GLfloat radius){

//   glTranslatef(.5,.5, 0.0f);  // Translate to (xPos, yPos)
   // Use triangular segments to form a circle
   glBegin(GL_TRIANGLE_FAN);
      glVertex2f(0.0f, 0.0f);       // Center of circle
      int numSegments = 100;
      GLfloat angle;
      for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
         angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
         glVertex2f((cos(angle) * radius), (sin(angle) * radius));
      }
   glEnd();
}
void output(GLfloat x, GLfloat y, char* text)
{
    glPushMatrix();
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glEnable(GL_BLEND);
         glEnable(GL_LINE_SMOOTH);
         glLineWidth(20.0);
    glTranslatef(x, y, 0);
//	glColor3f(0,0,0);
  //  glScalef(1/152.38, 1/152.38, 1/152.38);
	glScalef(1/152.38, 1/152.38, 1/152.38);
    for( char* p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}
void rectangle(){
	float q=0.5;
	float h=1.55;
	glPushMatrix();
	glTranslatef(0,0.4,0);
		glBegin(GL_QUADS);
      glColor3f(1.0f, 1.0f, 1.0f); 
      glVertex2d(-h, -q);
      glVertex2d( h, -q);
	  glVertex2d( h,  q);
      glVertex2d(-h,  q);
	   glEnd();
	   glPopMatrix();
}
void Text1() {
	//	char buffer[4];
 // string testx="1";
  
		char *text=new char[2];
		
		strcpy(text,"1");
	   
		glBegin(GL_QUADS);
      glColor3f(1.0f, 0.5f, 0.0f); //orange
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(0,0,0);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void Text2(){
	//	char buffer[4];
		char *text=new char[2];
		
		strcpy(text,"2");
	   
		glBegin(GL_QUADS);
      glColor3f(1.0f, 1.0f, 0.0f); 
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(0,0,0);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void Text3(){
	//	char buffer[4];
		char *text=new char[2];
		
		strcpy(text,"3");
		glBegin(GL_QUADS);
      glColor3f(0.0f, 1.0f, 0.0f); 
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(0,0,0);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void Text4(){
	//	char buffer[4];
		char *text=new char[2];
		
		strcpy(text,"4");
		glBegin(GL_QUADS);
      glColor3f(0.0f, 1.0f, 0.0f); 
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(1,1,1);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void Text5(){
	//	char buffer[4];
	char *text=new char[2];
		
		strcpy(text,"5");
		glBegin(GL_QUADS);
      glColor3f(1.0f, 0.5f, 0.0f); //orange
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(1,1,1);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void Text6(){
	
	char *text=new char[2];
		
		strcpy(text,"6");
		glBegin(GL_QUADS);
      glColor3f(0.0f, 1.0f, 1.0f); 
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(0,0,0);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void Text7(){
	//	char buffer[4];
		char *text=new char[2];
		
		strcpy(text,"7");
		glBegin(GL_QUADS);
      glColor3f(0.0f, 0.0f, 1.0f); 
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();	
	 /*  glBegin(GL_QUADS);
     glColor3f(1,1,1);
      glVertex2d(-0.2, -0.2);
      glVertex2d( 0.2, -0.2);
      glVertex2d( 0.2,  0.2);
      glVertex2d(-0.2,  0.2);
	   glEnd();*/
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(1,1,1);
		output(-0.3,-0.3,text); 
		
		glPopMatrix();
}

void Text8(){
	//	char buffer[4];
		char *text=new char[2];
		
		strcpy(text,"8");
		glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f); //purple
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(1,1,1);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void Text9(){
	//	char buffer[4];
	char *text=new char[2];
		
		strcpy(text,"9");
		glBegin(GL_QUADS);
	 glColor3f(0.5,0.0, 1.0f); //violet
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(1,1,1);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}

void Text0(){
	//	char buffer[4];
		char *text=new char[2];
		
		strcpy(text,"0");
		float q=0.02;
		glBegin(GL_QUADS);
      glColor3f(0.0f, 0.0f, 1.0f); 
      glVertex2d(-quadLength, -quadLength);
      glVertex2d( quadLength, -quadLength);
      glVertex2d( quadLength,  quadLength);
      glVertex2d(-quadLength,  quadLength);
	   glEnd();
	  
	/*   glBegin(GL_QUADS);
      glColor3f(0.0f, 0.0f, 0.0f); 
      glVertex2d(-q, -q);
      glVertex2d( q, -q);
      glVertex2d( q,  q);
      glVertex2d(-q,  q);
	   glEnd();*/
	   glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(0,0,0);
		output(-0.3,-0.3,text); 
		glPopMatrix();
}
void drawDigit(char value){
	switch (value){
	case '1':{Text1();break;}
	case '2':{Text2();break;}
	case '3':{Text3();break;}
	case '4':{Text4();break;}
	case '5':{Text5();break;}
	case '7':{Text7();break;}
	case '9':{Text9();break;}
	case '0':{Text0();break;}
	case '6':{Text6();break;}
	case '8':{Text8();break;}	
	}



}
void printlogfile(int id ,double time,struct tm *n){

	ofstream outdata;
	outdata.open("20151127_1730.txt", ios::app); 
	outdata << std::fixed<< id << "\t" << time <<endl;
	cout << std::fixed<< id << "\t\t" << time  << "\t\t" << n->tm_hour <<":"<<n->tm_min << ":" <<n->tm_sec<<endl;

}
void display() {
  
 glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
   glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
   glLoadIdentity();
	  time_t t = time(0);   // get time now.
    struct tm * now = localtime( & t );
   /* cout << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday << "  "<< now->tm_hour <<":"<<now->tm_min << ":" <<now->tm_sec;*/
	
	if (now->tm_hour==thour && now->tm_min==tmin && now->tm_sec==00){
	count_start=1;
	
	 t_start=std::chrono::high_resolution_clock::now();
		}

	 /*glPushMatrix();
	glTranslatef(ballX,ballY,0.0f);
      glColor3f(1.0f,0, 0.0f);  // Blue
     drawCircle(ballRadius);
    glPopMatrix();*/
 //  rectangle();
   // glTranslatef(ballX1, ballY1, 0.0f); 
   glColor3f(0.0f, 0.0, 1.0f);  // Red
   char buffer[10];
  //  drawCircle(0.2);
  // number=12345;

		//char *text=  itoa (number,buffer,10);
		snprintf(buffer,sizeof(buffer),"%d",number);
		if(strlen(buffer)==1){
				 glPushMatrix();
			glTranslatef((-1.1),0.5,0.0f);
			glScalef(1.1, 1.1, 1.1);
			drawDigit('0');
		 glPopMatrix();
		 glPushMatrix();
		   glTranslatef(-0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit('0');
	    glPopMatrix();
		  
		 glPushMatrix();
		   glTranslatef(0.0,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit('0');
	    glPopMatrix();
		  glPushMatrix();
		   glTranslatef(0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit('0');
	    glPopMatrix();
		 	  glPushMatrix();
		   glTranslatef(1.1,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[0]);
	    glPopMatrix();
		}
		else if(strlen(buffer)==2){
		 glPushMatrix();
			glTranslatef((-1.10),0.5,0.0f);
			glScalef(1.1, 1.1, 1.1);
			drawDigit('0');
		 glPopMatrix();
		 glPushMatrix();
		   glTranslatef(-0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit('0');
	    glPopMatrix();
		  
		 glPushMatrix();
		   glTranslatef(0.0,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit('0');
	    glPopMatrix();
		  glPushMatrix();
		   glTranslatef(0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[0]);
	    glPopMatrix();
		 	  glPushMatrix();
		   glTranslatef(1.1,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[1]);
	    glPopMatrix();
		}
		else if(strlen(buffer)==3){
		glPushMatrix();
			glTranslatef((-1.10),0.5,0.0f);
			glScalef(1.1, 1.1, 1.1);
			drawDigit('0');
		 glPopMatrix();
		 glPushMatrix();
		   glTranslatef(-0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit('0');
	    glPopMatrix();
		  
		 glPushMatrix();
		   glTranslatef(0.0,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[0]);
	    glPopMatrix();
		  glPushMatrix();
		   glTranslatef(0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[1]);
	    glPopMatrix();
		 	  glPushMatrix();
		   glTranslatef(1.1,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[2]);
		}
		else if(strlen(buffer)==4){
		 glPushMatrix();	
		 glTranslatef((-1.1),0.5,0.0f);
		 glScalef(1.1, 1.1, 1.1);
			drawDigit('0');
		 glPopMatrix();
		 glPushMatrix();
		   glTranslatef(-0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[0]);
	    glPopMatrix();
		  
		 glPushMatrix();
		   glTranslatef(0.0,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[1]);
	    glPopMatrix();
		  glPushMatrix();
		   glTranslatef(0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[2]);
	    glPopMatrix();
		 	  glPushMatrix();
		   glTranslatef(1.1,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[3]);
		}
  	else if(strlen(buffer)==5){
		 glPushMatrix();
			glTranslatef((-1.1),0.5,0.0f);
			glScalef(1.1, 1.1, 1.1);
			drawDigit(buffer[0]);
		 glPopMatrix();
		 glPushMatrix();
		   glTranslatef(-0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[1]);
	    glPopMatrix();
		  
		 glPushMatrix();
		   glTranslatef(0.0,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[2]);
	    glPopMatrix();
		  glPushMatrix();
		   glTranslatef(0.55,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[3]);
	    glPopMatrix();
		 	  glPushMatrix();
		   glTranslatef(1.1,0.5,0.0f);
		   glScalef(1.1, 1.1, 1.1);
	      drawDigit(buffer[4]);
	    glPopMatrix();
		}

		
 //glColor3f(0.5,0.0, 1.0f);  
 //   drawCircle(0.5);

   ballX += xSpeed;
   ballY += ySpeed;
   ballX1 += xSpeed1;
   ballY1 += ySpeed1;
   // Check if the ball exceeds the edges
   if (ballX > (ballXMax)) {
      ballX = ballXMax;
      xSpeed = -xSpeed;
   } else if (ballX < (ballXMin)) {
      ballX = ballXMin;
      xSpeed = -xSpeed;
   }
   if (ballY > ballYMax) {
      ballY = ballYMax;
      ySpeed = -ySpeed;
   } else if (ballY < ballYMin) {
      ballY = ballYMin;
      ySpeed = -ySpeed;
   }
  if ( (ballX>(-0.2) && ballX < 0.2) &&
		  (ballY>-0.2)&&ballY < 0.2) {
	xSpeed=-xSpeed;
	ySpeed=-ySpeed; 

	if (count_start==1){
	clock_t end1=clock();
	 auto end=std::chrono::high_resolution_clock::now();
	 auto diff= end - t_start;
	
	  time_t t1 = time(0);   // get time now.
    struct tm * now1 = localtime( & t1 );
	
	double elapsed_milsecs=std::chrono::duration<double,std::milli>(end - t_start).count();
	//cout << ++number<< "  " <<"elapsed_secs=" << elapsed_secs <<endl;
//	cout << ++number<< "  " <<"elapsed_secs=" << elapsed_milsecs/1000 << "     " 
	double time_e= elapsed_milsecs/1000.0 ;
//	cout << ++number<< "  " <<"elapsed_secs=" << time_e << "     "  
//		 << (now->tm_year + 1900) << '-' 
//         <<  now->tm_mday << "  "<< now->tm_hour <<":"<<now->tm_min << ":" <<now->tm_sec<<endl;

	printlogfile(++number,time_e,now);
	publish_result(number,time_e);
	}
  }

	
//glFlush();
 glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
}
void reshape(GLsizei width, GLsizei height) {

   if (height == 0) height = 1;               
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
  // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset the projection matrix
   if (width >= height) {
      clipAreaXLeft   = -1.0 * aspect;
      clipAreaXRight  = 1.0 * aspect;
      clipAreaYBottom = -1.0;
      clipAreaYTop    = 1.0;
   } else {
      clipAreaXLeft   = -1.0;
      clipAreaXRight  = 1.0;
      clipAreaYBottom = -1.0 / aspect;
      clipAreaYTop    = 1.0 / aspect;
   }
   gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
   ballXMin = clipAreaXLeft + ballRadius;
   ballXMax = clipAreaXRight - ballRadius;
   ballYMin = clipAreaYBottom + ballRadius;
   ballYMax = clipAreaYTop - ballRadius;

      ballXMin1 = clipAreaXLeft + ballRadius1;
   ballXMax1 = clipAreaXRight - ballRadius1;
   ballYMin1 = clipAreaYBottom + ballRadius1;
   ballYMax1 = clipAreaYTop - ballRadius1;
   //ballXQuadleft=(quadLength)-ballRadius;
   //ballXQuadRight=(quadLength)+ballRadius;
   //ballYQuadBottom=(quadLength)+ballRadius;
   //ballYQuadTop=(quadLength)-ballRadius;
  //  std::cout << ballX*(clipAreaXLeft-clipAreaXRight)<<  "    " << ballY <<endl;
    
}
 
/* Called back when the timer expired */
void Timer(int value) {
   glutPostRedisplay();    // Post a paint request to activate display()
   glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   cout <<"Enter time to start(current hours and  minutes):";
   cin >> thour >>tmin;

   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
   glutCreateWindow(title);      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutTimerFunc(0, Timer, 0);   // First timer call immediately
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 0;
}
