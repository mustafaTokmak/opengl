/* This program illustrates the use of the glut library for
interfacing with a Window System */

/* The program opens a window, clears it to black,
then draws a box at the location of the mouse each time the
left button is clicked. The right button exits the program

The program also reacts correctly when the window is
moved or resized by clearing the new window to black*/


#include <time.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

/* globals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */

typedef struct {
      GLint x;
      GLint y;
} Point2D;

typedef struct {
      GLubyte r;
      GLubyte g;
      GLubyte b;
} Color;

typedef struct {
	Point2D center;
        Color color;
} Square;


Square squareArray[10000];
int numSquares=0;

void makeSquare(int x, int y)
{

	y = wh - y;
        squareArray[numSquares].center.x = x;
	squareArray[numSquares].center.y = y;
        squareArray[numSquares].color.r = (GLubyte) rand() % 256;
        squareArray[numSquares].color.g = (GLubyte) rand() % 256;
        squareArray[numSquares++].color.b = (GLubyte) rand() % 256;

        glutPostRedisplay();
}


/* rehaping routine called whenever window is resized
or moved */

void myReshape(GLsizei w, GLsizei h)
{

	/* adjust clipping box */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* adjust viewport and clear */

	glViewport(0, 0, w, h);

	/* set global size for use by drawing routine */

	ww = w;
	wh = h;
}

void myinit(void)
{

	glViewport(0, 0, ww, wh);

	/* Pick 2D clipping window to match size of screen window
	This choice avoids having to scale object coordinates
	each time window is resized */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)ww, 0.0, (GLdouble)wh);

	/* set clear color to black and clear window */

	glClearColor(0.0, 0.0, 0.0, 1.0);


}

void myKeyboard(unsigned char key, int x, int y)
{
	if ((key == 'Q') || (key == 'q'))
		exit(0);
}


void myMouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		exit(0); /*terminate the program through OpenGL */
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		makeSquare(x, y);
}

/* display callback required by GLUT 3.0 */

void myDisplay(void)
{  
        int i, x, y; 
	glClear(GL_COLOR_BUFFER_BIT);
        for (i=0; i <numSquares; i++) {
		x=squareArray[i].center.x;
		y=squareArray[i].center.y;
		glColor3ub(squareArray[i].color.r, squareArray[i].color.g, squareArray[i].color.b);
		glBegin(GL_POLYGON);
		   glVertex2f(x + size, y + size);
	           glVertex2f(x - size, y + size);
	           glVertex2f(x - size, y - size);
	           glVertex2f(x + size, y - size);
	         glEnd();
        }
        glFlush();
}

void myMenu(int id) {
	if (id == 1)
		size = size * 2;
	else if (id == 2 && size > 1)
		size = size / 2;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{


	int menu_id;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ww, wh);
	glutCreateWindow("square");
	myinit();
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	glutMotionFunc(makeSquare);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);

	menu_id = glutCreateMenu(myMenu);
	glutAddMenuEntry("Increase square size", 1);
	glutAddMenuEntry("Decrease square size", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	srand(time(NULL));

	glutMainLoop();

}
