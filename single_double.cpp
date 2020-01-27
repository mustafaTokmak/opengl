/*
*  double.c
*  This program demonstrates double buffering for
*  flicker-free animation.  The left and right mouse
*  buttons start and stop the spinning motion of the square.
*/

#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

GLfloat theta = 0.0;


void myDisplay(void)
{
	GLfloat thetar;
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	thetar = theta * (2 * 3.14159) / 360.0;
	/* convert degrees to radians */
	glVertex2f(cos(thetar), sin(thetar));
	glVertex2f(-sin(thetar), cos(thetar));
	glVertex2f(-cos(thetar), -sin(thetar));
	glVertex2f(sin(thetar), -cos(thetar));
	glEnd();
	//    glFlush(); /*single buffering */
	glutSwapBuffers(); /* double buffering */
}


void myIdle(void)
{
	theta = theta + 2;
	if (theta >= 360.0)
		theta -= 360.0;
	glutPostRedisplay();
}

void myinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glShadeModel(GL_FLAT);
}

void myMouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)  glutIdleFunc(myIdle);
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)   glutIdleFunc(NULL);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-3.0, 3.0, -3.0, 3.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myTimeout(int id) {

	theta = theta + 2;
	if (theta >= 360.0)
		theta -= 360.0;
	glutTimerFunc(20, myTimeout, 0);
	glutPostRedisplay();

}
/*  Main Loop
*  Open window with initial window size, title bar,
*  RGBA display mode, and handle input events.
*/
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// TRY single buffering
	//	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 

	glutCreateWindow("double buffered");
	myinit();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutIdleFunc(myIdle);
	//glutTimerFunc(20, myTimeout, 0);
	glutMouseFunc(myMouse);



	glutMainLoop();

}
