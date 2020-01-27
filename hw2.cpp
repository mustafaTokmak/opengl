#include <stdlib.h>			// standard definitions

#include <stdio.h>			// C I/O (for sprintf) 
#include <math.h>			// standard definitions

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// global variable declarations

GLint windowHeight=600, windowWidth=500;
GLint totalLane = 24+1;

//-----------------------------------------------------------------------
//	Sets up some default OpenGL values.
//-----------------------------------------------------------------------

void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);		// background color
    // glShadeModel(GL_FLAT);		// flat shading
    glShadeModel(GL_FLAT);		// smooth shading

}


//-----------------------------------------------------------------------
// reshape callback function
//	This is called each time the window is reshaped
//-----------------------------------------------------------------------
void myReshape(int winWidth, int winHeight) 
{
    windowHeight = winHeight;
    windowWidth = winWidth;
  // update projection
  // try this instead, what is the difference?
  //glViewport (0, 0, 300, 300); 
  glViewport (0, 0, winWidth,  winHeight);  // update the viewport
  glMatrixMode(GL_PROJECTION);              // update the projection
  glLoadIdentity();               

  gluOrtho2D(0.0, 1.0, 0.0, 1.0);           // map unit square to viewport
  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();                      // request redisplay
  
  
}
void drawCar(double x,double y)
{
    
}
void drawTruck(double x,double y)
{

}



//-----------------------------------------------------------------------
// display callback function
//	This is called each time application needs to redraw itself.
//-----------------------------------------------------------------------
double getRoadSideWalkSize(){
    double sidewalks_size = 1/(windowHeight/(1.0*totalLane));
    return sidewalks_size;
}
void drawSideWalks()
{   
    // number of lane of each road
    int numberOfLane[] = {0,3,4,4,3,4};
    // total number of lane of top to down
    int totalNumberOfLane[] = {0,0,0,0,0,0};
    int sum = 0;
    for( int i = 0; i < 6; i++) {
        sum = sum + numberOfLane[i]+1;
        totalNumberOfLane[i] = sum;
    }
    double sidewalks_size = getRoadSideWalkSize();
    
    for( int i = 0; i < 6; i++) {
        glColor3f(0.0, 0.0, 0.0);       // set color to red
        glBegin(GL_POLYGON);            // list the vertices to draw a diamond
        glVertex2f(0.0, 1.0-totalNumberOfLane[i]*sidewalks_size);
        glVertex2f(0.0, 1.0-totalNumberOfLane[i]*sidewalks_size+sidewalks_size);
        glVertex2f(1.0, 1.0-totalNumberOfLane[i]*sidewalks_size+sidewalks_size);
        glVertex2f(1.0, 1.0-totalNumberOfLane[i]*sidewalks_size);
        glEnd();
        //printf("%.7lf\n",1.0-numberOfLane[i]*sidewalks_size);
    }
    
    
    
}


void drawBrokenLine(double y)
{   
    int numberOfBrokenLines = 20.0;
    double brokenLineHeight = 1.0/windowHeight;
    double brokenLineWidth = 1.0/(2*numberOfBrokenLines);
    printf("%.7lf\n",brokenLineHeight);
    for( int i = 0; i < 2*numberOfBrokenLines; i++) {
        if(i % 2==0){
            glColor3f(0.0, 0.0, 0.0);       // set color to red
            glBegin(GL_POLYGON);            // list the vertices to draw a diamond
            glVertex2f(0.0+i*brokenLineWidth, y);
            glVertex2f(0.0+i*brokenLineWidth, y-brokenLineHeight);
            glVertex2f(0.0+(i+1)*brokenLineWidth, y-brokenLineHeight);
            glVertex2f(0.0+(i+1)*brokenLineWidth, y);
            glEnd();
        }
    }
    
}
void drawRoad()
{   
    double roadSize = getRoadSideWalkSize();
    for( int i = 0; i < totalLane; i++) {
        double y = 1.0*i;
        y = y*roadSize;
        drawBrokenLine(y);        
    }


}



void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);       // clear the window
   
    
    
    drawRoad();
    
    drawSideWalks();
    //drawBrokenLine(0.3);
    
     
	
     glFlush();				// force OpenGL to render now

     glutSwapBuffers();			// swap buffers
}

//-----------------------------------------------------------------------
// keyboard callback function
//	This is called whenever a keyboard key is hit.
//-----------------------------------------------------------------------

void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
      case 'q':
		  exit(0);			// exit
	// other keyboard events may follow
   
	}

    glutPostRedisplay();		// request redisplay
}
// called if mouse click
void myMouse(int b, int s, int x, int y) {
	switch  ( b ) {    // b indicates the button
	case GLUT_LEFT_BUTTON:
		if (s == GLUT_DOWN)      // button pressed
			;
		else if (s == GLUT_UP)   // button released
			;
		break;
		//... other button events may follow
	}
}

// Called if timer event occurs
void myTimeOut(int id) {
	// advance the state of animation incrementally

	glutPostRedisplay();			  // request redisplay
	glutTimerFunc(2000, myTimeOut, 0);  // request next timer event
}

//-----------------------------------------------------------------------
// main program
//	Where everything begins.
//-----------------------------------------------------------------------

int main(int argc, char **argv)
{
    				
    glutInit(&argc, argv);   //Initialize glut and gl
    glutInitDisplayMode(		
		GLUT_DOUBLE |		// double buffering
		GLUT_RGB );		    // RGB color mode

    glutInitWindowSize(windowWidth,windowHeight);   // 400x400 window
    glutInitWindowPosition(0,0);    // upper left
    glutCreateWindow("My First OpenGL Program"); 	// create the window

	//register callbacks
    glutDisplayFunc(myDisplay);		//  call myDisplay to redraw window
    glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
    glutReshapeFunc(myReshape);     // call myReshape if window is resized
    glutMouseFunc(myMouse);         // call in mouse event 
    glutTimerFunc(2000, myTimeOut, 0);

    myInit();				// our own initializations

    glutMainLoop();			// pass control to GLUT, start it running
    return 0;               // ANSI C expects this
}
