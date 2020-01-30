/**
 * This program show on how to use timer function in OpenGL
 * 
 * For more tutorial, see: 
 * http://60hz.csse.uwa.edu.au/workshop/workshop0/workshop2.html
 */


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <ctime>
#include <cstdlib>

#include <iostream>

using namespace std;

int ispause = 0;
int angle = 1;

static void Timer(int value){
    angle += 0.1;
    glutPostRedisplay();
    // 100 milliseconds
    if(!ispause){
        glutTimerFunc(100, Timer, value);
    }


}





void init(){
    glClearColor(1.0,1.0,1.0,1.0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 01.0);

    glColor3f(0.9,0.9,0.1);
    glutSolidTeapot(0.5);

    glFlush();

}
void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        
        case 'q':
		    exit(0);	
            break;
        case 'p':
            ispause = 1;
            break;
	}

    glutPostRedisplay();		// request redisplay
}

void myMouse(int b, int s, int x, int y) {
	switch  ( b ) {    // b indicates the button
	case GLUT_LEFT_BUTTON:
		if (s == GLUT_DOWN){      // button pressed
            angle += 0.1;
            glutPostRedisplay();	
        }	//glutTimerFunc(NULL);
		break;
		//... other button events may follow
	}
}


int main(int argc, char *argv[]){


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(400,300);
    glutInitWindowPosition(200,100);
    glutCreateWindow("Simulation App");
    glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
    glutMouseFunc(myMouse);

    init();
    glutDisplayFunc(display);

    Timer(0);

    glutMainLoop();



    return 0;
}