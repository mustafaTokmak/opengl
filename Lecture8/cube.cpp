/* Rotating cube with color interpolation */

/* Demonstration of use of homogeneous coordinate 
transformations and simple data structure for representing
cube */

/*colors are assigned to the vertices */
/*Cube is centered at origin  */

#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 8; 

point4  tvertices[8]; //transformed vertices

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4(-0.5, -0.5,  0.5, 1.0),
	point4(-0.5,  0.5,  0.5, 1.0),
	point4(0.5,  0.5,  0.5, 1.0),
	point4(0.5, -0.5,  0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5,  0.5, -0.5, 1.0),
	point4(0.5,  0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

// RGBA olors
color4 colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(0.0, 1.0, 1.0, 1.0)   // cyan
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };




void quad(int a, int b, int c , int d)
{
	
/* draw a polygon via list of vertices */

 	glBegin(GL_POLYGON);
		glColor3f(colors[a].x, colors[a].y, colors[a].z);
                glVertex3f(tvertices[a].x, tvertices[a].y, tvertices[a].z);
		glColor3f(colors[b].x, colors[b].y, colors[b].z);
		glVertex3f(tvertices[b].x, tvertices[b].y, tvertices[b].z);
		glColor3f(colors[c].x, colors[c].y, colors[c].z);
		glVertex3f(tvertices[c].x, tvertices[c].y, tvertices[c].z);
		glColor3f(colors[d].x, colors[d].y, colors[d].z);
		glVertex3f(tvertices[d].x, tvertices[d].y, tvertices[d].z);
	glEnd();
																								      
}


void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

void display(void)
{
/* display callback, clear frame buffer and z buffer,
   rotate cube and draw, swap buffers */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* OLD OpenGL way */
    /* load identity to modelview then postmultiply with rotation matrices */
    /* Modelview matrix is part of state */

    /*	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0); */

    
    /* new OpenGL make your own matrices */
    /* construct ctm matrix */
    /* for now multiply ctm with vertices in the application program */
    mat4  ctm = (RotateX(Theta[Xaxis]) *
		RotateY(Theta[Yaxis]) *
		 RotateZ(Theta[Zaxis]));// * Translate(0.25, 0.25, 0.25) * Scale(0.5, 0.5, 0.5));

    /*transform vertices by multiplying with ctm */

    for (int i = 0; i < NumVertices; ++i) {
	tvertices[i] = ctm * vertices[i];
    }

    colorcube();

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
idle(void)
{
	Theta[Axis] += 0.5;

	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}

	glutPostRedisplay();
}

void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}



void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

//    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
}

int
main(int argc, char **argv)
{
    glutInit(&argc, argv);

/* need both double buffering and z buffer */

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("colorcube");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
    glutMainLoop();
}





