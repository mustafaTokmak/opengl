#include <stdlib.h>			// standard definitions

#include <stdio.h>			// C I/O (for sprintf) 
#include <math.h>			// standard definitions

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// global variable declarations

GLint windowHeight, windowWidth;


//-----------------------------------------------------------------------
//	Sets up some default OpenGL values.
//-----------------------------------------------------------------------

void myInit()
{
    glClearColor(0.5, 0.5, 0.5, 0.3);		// background color
    // glShadeModel(GL_FLAT);		// flat shading
    // glShadeModel(GL_SMOOTH);		// smooth shading

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

  gluOrtho2D(0.0, 1.0, 0.1, 1.0);           // map unit square to viewport
  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();                      // request redisplay
  
  
}



//-----------------------------------------------------------------------
// display callback function
//	This is called each time application needs to redraw itself.
//-----------------------------------------------------------------------

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);       // clear the window
   
    glColor3f(0.3, 0.3, 0.0);       // set color to red
    glBegin(GL_POLYGON);            // list the vertices to draw a diamond
	glVertex2f(0.90, 0.50);
	glVertex2f(0.50, 0.90);
	glVertex2f(0.10, 0.50);
	glVertex2f(0.50, 0.10);
     glEnd();

     glColor3f(0.0, 0.0, 1.0);       // set color to blue
     glBegin(GL_POLYGON);            // list the vertices to draw a square
	glVertex2f(0.25, 0.75);
	glVertex2f(0.25, 0.25);
	glVertex2f(0.75, 0.25);
	glVertex2f(0.75, 0.75);
     glEnd();
     
	
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
	glutTimerFunc(20, myTimeOut, 0);  // request next timer event
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

    glutInitWindowSize(400, 400);   // 400x400 window
    glutInitWindowPosition(0,0);    // upper left
    glutCreateWindow("My First OpenGL Program"); 	// create the window

	//register callbacks
    glutDisplayFunc(myDisplay);		//  call myDisplay to redraw window
    glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
    glutReshapeFunc(myReshape);     // call myReshape if window is resized
    glutMouseFunc(myMouse);         // call in mouse event 
    glutTimerFunc(20, myTimeOut, 0);

    myInit();				// our own initializations

    glutMainLoop();			// pass control to GLUT, start it running
    return 0;               // ANSI C expects this
}
