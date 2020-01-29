#include <stdlib.h>			// standard definitions

#include <stdio.h>			// C I/O (for sprintf) 
#include <math.h>			// standard definitions
#include <time.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// global variable declarations

GLint windowHeight=600, windowWidth=500;
const int totalNumberOfLane = 24;
GLint totalLane = totalNumberOfLane+1;
GLdouble vehicleX = 0.1;
GLdouble vehicleV = 0.01;
GLdouble agentX = 0.5;
GLint agentRoad = 0;
GLint agentLane = 0;
GLint agentDirection = 1;
GLint timerDifficulty = 100;
GLint score = 0;
//

//-----------------------------------------------------------------------
//	Sets up some default OpenGL values.
//-----------------------------------------------------------------------
//all even lane is right to left 
// all odd lane is left to right
struct lane
{
    int road;
    int laneNo;
    int velocity; // -5,5
};

struct vehicle 
{  
   //car type is 0 
   //truck type is 1 
   int type;
   lane laneRef;
   double xAxis;

};

// lanes + sidewalks
struct lane laneArr[totalNumberOfLane];
int totalVehicle = totalNumberOfLane*25;
struct vehicle vehicleArr[totalNumberOfLane*25];
int vehicleIndexArr[totalNumberOfLane];

int vehicleIndex = 0;

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
double getLaneSize(){
    double sidewalks_size = 1/(windowHeight/(1.0*totalLane));
    return sidewalks_size;
}
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
double calculateVehicleYaxis(int road,int lane)
{   
    //roads and lanes indexs down to up
    
    //TODO this array can be global or function
    //all road have sidewalk to bottom
    //first road have 0 lane
    double laneSize = getLaneSize();
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};
    int sum = 0;
    for( int i = 0; i < road; i++) {
        sum = sum + numberOfLaneOfRoads[i]+1;
    }
    double y = ((sum+lane)-1.0)*laneSize+laneSize/2.0;
    return y;
}
double calculateAgentYaxis(int road,int lane)
{
    //roads and lanes indexs down to up
    double laneSize = getLaneSize();
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};
    int sum = 0;
    for( int i = 0; i < road; i++) {
        sum = sum + numberOfLaneOfRoads[i]+1;
    }
    //if lane == 0 means sidewalk
    double y = 0;
    if(lane == 0){
        y = (sum + numberOfLaneOfRoads[road])*laneSize+laneSize/2.0;
    }
    else
    {
        y = ((sum+lane)-1.0)*laneSize+laneSize/2.0;
    }
    
    return y;
}
void drawAgent(int road,int lane,double x,int direction){
    if (direction != 1 && direction != -1){
        printf("ERROR : direction must 1 or -1\n");
        exit(0);
    }
    double laneSize = getLaneSize();
    double agentHeight = laneSize*0.8;
    double y = calculateAgentYaxis(road,lane);
    glColor3f(0.0, 1.0, 0.0);       // set color to red
    glBegin(GL_POLYGON);            // list the vertices to draw a diamond
    //direction 1 down to up
    //direction -1 up to down
    if(direction == 1){
        glVertex2f(x - agentHeight/2.0, (y - agentHeight/2.0));
        glVertex2f(x + agentHeight/2.0, (y - agentHeight/2.0));
        glVertex2f(x, (y + agentHeight/2.0));
        
    }
    else{
        glVertex2f(x - agentHeight/2.0, (y + agentHeight/2.0));
        glVertex2f(x + agentHeight/2.0, (y + agentHeight/2.0));
        glVertex2f(x, (y - agentHeight/2.0));
    }
    glEnd();
    
}
void drawVehicle(int type,int road,int lane,double x)
{   
    double laneSize = getLaneSize();
    double vehicleHeight = laneSize*0.8;
    double vehicleWidth = vehicleHeight*((1.0*windowHeight)/(1.0*windowWidth));
    if(type == 1){
        vehicleWidth = vehicleWidth*2;
    }
    double y = calculateVehicleYaxis(road,lane);

    glColor3f(0.0, 1.0, 0.0);       // set color to red
    glBegin(GL_POLYGON);            // list the vertices to draw a diamond
    // * - > *
    // ^     |
    // |      
    // *
    glVertex2f(x - vehicleWidth/2.0, (y - vehicleHeight/2.0));
    glVertex2f(x - vehicleWidth/2.0, (y + vehicleHeight/2.0));
    glVertex2f(x + vehicleWidth/2.0, (y + vehicleHeight/2.0));
    glVertex2f(x + vehicleWidth/2.0, (y - vehicleHeight/2.0));
    glEnd();
}






//-----------------------------------------------------------------------
// display callback function
//	This is called each time application needs to redraw itself.
//-----------------------------------------------------------------------

void drawSideWalks()
{   
    // number of lane of each road
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};
    // total number of lane of top to down
    int totalNumberOfLane[] = {0,0,0,0,0,0};
    int sum = 0;
    for( int i = 0; i < 6; i++) {
        sum = sum + numberOfLaneOfRoads[i]+1;
        totalNumberOfLane[i] = sum;
    }
    double sidewalks_size = getLaneSize();
    
    for( int i = 0; i < 6; i++) {
        glColor3f(0.0, 0.0, 0.0);       // set color to red
        glBegin(GL_POLYGON);            // list the vertices to draw a diamond
        glVertex2f(0.0, totalNumberOfLane[i]*sidewalks_size);
        glVertex2f(0.0, totalNumberOfLane[i]*sidewalks_size-sidewalks_size);
        glVertex2f(1.0, totalNumberOfLane[i]*sidewalks_size-sidewalks_size);
        glVertex2f(1.0, totalNumberOfLane[i]*sidewalks_size);
        glEnd();
        //printf("%.7lf\n",1.0-numberOfLane[i]*sidewalks_size);
    }
    
    
    
}


void drawBrokenLine(double y)
{   
    int numberOfBrokenLines = 20.0;
    double brokenLineHeight = 1.0/windowHeight;
    double brokenLineWidth = 1.0/(2*numberOfBrokenLines);
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
    double roadSize = getLaneSize();
    for( int i = 0; i < totalLane; i++) {
        double y = 1.0*i;
        y = y*roadSize;
        drawBrokenLine(y);        
    }


}
void drawCoin(int road,int lane,double x)
{
    //filled circle
    float x1,y1,x2,y2;
    float angle;
    double radius=(getLaneSize()/2.0)*0.8;

    x1 = x;
    y1 = calculateVehicleYaxis(road,lane);
    glColor3f(1.0,1.0,0.0);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);

    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius*((1.0*windowHeight)/(1.0*windowWidth));
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }

    glEnd();
}
void createCoin()
{
    double coinXaxis = ((rand() % 500)*1.0) / 500;
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3}; 
    int coinRoad = rand() % 6;
    int coinLane;
    if( coinRoad == 0 ){
        coinLane = 0;
    } 
    else{
        coinLane = rand() % numberOfLaneOfRoads[coinRoad];
    }
    //add coin(coinRoad,coinLane,coinXaxis)

}
int isVehicleAgentCollision()
{   
    double vehicleHeight  = getLaneSize();
    double vehicleWidth = vehicleHeight*((1.0*windowHeight)/(1.0*windowWidth));
    // for all vehicles
    for(int i=0;i<totalNumberOfLane*25;i++){
        if (vehicleArr[i].type != -1){
            getAgent
        }     
    }
    //get agent x and y 
    //get coin x and y 
    //if y1== y2 and 
    //if vehicle truck and |x1-x2| < vehicleWidth*1.5
    //if vehicle car and |x1-x2| < vehicleWidth*1.0
    // collision : game over

}
void printScore()
{

}
int isCoinAgentCollision()
{
    // for all coin 
    //get agent x and y 
    //get coin x and y 
    //if y1== y2 and |x1-x2| < getLaneSize() * (0.8 - 0.1)
    // collision : score add remove coin from coin array
}

int isVehicleCollision()
{
    //for all vehicle for all vehicle
    return 0;
}
void initLanes()
{
    int index = 0;
    int laneV; // from -5 to 5 = (rand() % 6 ) -10;
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};
    
    for(int i =0;i < 6; i++){
       
        for(int j = 0; j < numberOfLaneOfRoads[i]+1; j++){
            lane lanetemp;
            lanetemp.laneNo = j;
            lanetemp.road = i;
            if(j==0)
                lanetemp.velocity = 0;
            else{
                int v = (rand() % 5 ) + 1;
                int dir = ((rand() %2) * 2) -1;
                lanetemp.velocity = v*dir;
            }
                
            laneArr[index] = lanetemp;
            index += 1;
        }
    }
    
}

void addVehicle(struct lane lane){
    vehicle vehicleTemp;
    int laneNo = lane.laneNo;
    if(laneNo ==0)
        return;
    int roadNo = lane.road;
    vehicleTemp.laneRef = lane;
    vehicleTemp.type = rand() % 2;
    if (lane.velocity < 0)
        vehicleTemp.xAxis = 1.1;//((rand() % 10)*1.0)/10;  //-0.1;
    else
        vehicleTemp.xAxis = -0.1;
    vehicleTemp.xAxis = ((rand() % 10)*1.0)/10;
    vehicleArr[vehicleIndex] = vehicleTemp;
    vehicleIndexArr[roadNo*4+laneNo] = vehicleIndex;
    vehicleIndex = (vehicleIndex + 1) % totalVehicle;
    
}

void initvehicles()
{
    for(int i =0; i < totalNumberOfLane*25;i++){
        vehicleArr[i].type=-1;
    }
    for(int i=0;i<totalNumberOfLane;i++){
        struct lane l = laneArr[i];
        addVehicle(l);
    }
    for(int i=0;i<totalNumberOfLane;i++){
        struct lane l = laneArr[i];
        addVehicle(l);
    }
    for(int i=0;i<totalNumberOfLane;i++){
        struct lane l = laneArr[i];
        addVehicle(l);
    }
    for(int i=0;i<totalNumberOfLane*25;i++){
        if (vehicleArr[i].type != -1){
            printf("%d",i);
            printf("bilmem ne\n");
        }
            
    }
}
void drawAllVehicles(){
    for(int i=0;i<totalNumberOfLane*25;i++){
        if (vehicleArr[i].type != -1){
            drawVehicle(vehicleArr[i].type,vehicleArr[i].laneRef.road,vehicleArr[i].laneRef.laneNo,vehicleArr[i].xAxis);
        }     
    }
}
int isCreateVehicleAvaliable(int road,int lane){

}
void createNewVehicle()
{
        
}

void updateVehicles(){
    double laneSize = getLaneSize();
    double vehicleHeight = laneSize*0.8;
    double vehicleWidth = vehicleHeight*((1.0*windowHeight)/(1.0*windowWidth));
    double baseV = 1/((1.0*windowWidth)/vehicleWidth)*25.0 * 3;
    //baseV is a 1 pixel 
    //
    double v = baseV * 1;
    
    for(int i=0;i<totalNumberOfLane*25;i++){
        if (vehicleArr[i].type != -1){
            double xAxis = (vehicleArr[i].xAxis + (baseV *vehicleArr[i].laneRef.velocity));
            if(xAxis > -0.1 && xAxis < 1.1){
                vehicleArr[i].xAxis = xAxis;  
            }
            else{
                vehicleArr[i].type = -1;
            }
            
        }     
    }
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);       // clear the window
   
    drawRoad();
    
    drawSideWalks();

    drawAllVehicles();
    
    drawAgent(agentRoad,agentLane,agentX,agentDirection);
    drawCoin(1,1,0.7);
    drawCoin(3,2,0.1);

    drawCoin(2,1,0.3);
    drawCoin(4,3,0.8);

    drawCoin(5,2,0.5);


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
		    exit(0);	
            break;
        case '1':
            timerDifficulty = 100 - 1*11;
            break;
        case '2':
            timerDifficulty = 100 - 2*11;
            break;
        case '3':
            timerDifficulty = 100 - 3*11;
            break;
        case '4':
            timerDifficulty = 100 - 4*11;
            break;
        case '5':
		    timerDifficulty = 100 - 5*1;
            break;
        case '6':
            timerDifficulty = 100 - 6*11;
            break;
        case '7':
            timerDifficulty = 100 - 7*11;
            break;
        case '8':
		    timerDifficulty = 100 - 8*11;
            break;
        case '9':
            timerDifficulty = 100 - 9*11;		// exit
            break;
	// other keyboard events may follow
	}

    glutPostRedisplay();		// request redisplay
}
// called if mouse click
void myMouse(int b, int s, int x, int y) {
	switch  ( b ) {    // b indicates the button
	case GLUT_LEFT_BUTTON:
		if (s == GLUT_DOWN)      // button pressed
			;//glutTimerFunc(NULL);
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

static void Timer(int value){
    updateVehicles();
    //vehicleX += vehicleV; 
    
    glutPostRedisplay();
    // 100 milliseconds
    glutTimerFunc(timerDifficulty, Timer, 0);
}
//from stackoverflow
void updateAgent(int key)
{
    double laneSize = getLaneSize();
    double agentHeight = laneSize*0.8;
    double agentWidth = agentHeight;
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};    
    
    if(key == GLUT_KEY_LEFT){
        if(agentX - agentWidth/2.0 > agentWidth)
            agentX = agentX - agentWidth/2.0;
    }       
    else if(key == GLUT_KEY_RIGHT){
        if(agentX + agentWidth/2.0 < 1.0 - agentWidth)
            agentX = agentX + agentWidth/2.0;
    }    
    else if(key == GLUT_KEY_DOWN){
        int numberOfRoad = 5;
        if(agentDirection == -1){
            if(agentLane == 0){
                int totalLane = numberOfLaneOfRoads[agentRoad];
                if(agentRoad > 0){
                    agentLane = totalLane;
                }
            }
            else{
                int totalLane = numberOfLaneOfRoads[agentRoad];
                if(agentLane > 1){
                    agentLane = agentLane - 1;
                }
                else{
                    agentLane = 0;
                    if(agentRoad > 0)
                        agentRoad = agentRoad - 1;
                    if(agentRoad == 0){
                        agentDirection = 1;
                        score += 1;
                    }
                }

            }
        }
    }
    else if(key == GLUT_KEY_UP){
        int numberOfRoad = 5;
        if(agentDirection == 1){
            if(agentLane == 0){
                if(agentRoad < numberOfRoad){
                    agentLane = 1;
                    agentRoad = agentRoad + 1;
                } 
            }
            else{
                int totalLane = numberOfLaneOfRoads[agentRoad];
                if(agentLane < totalLane){
                    agentLane = agentLane + 1;
                }
                else{
                    agentLane = 0;
                    if(agentRoad == numberOfRoad)
                        agentDirection = -1;
                }

            }
        }
    }
    
    
}
void catchKey(int key, int x, int y)
{   
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};
    int laneDirection = rand() % 5;
    printf(" %d \n", laneDirection); 
    updateAgent(key);
}

int main(int argc, char **argv)
{   
    srand ( time(NULL) );
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
    Timer(0);
    myInit();				// our own initializations
    glutSpecialFunc(catchKey);
    initLanes();
    initvehicles();
    glutMainLoop();			// pass control to GLUT, start it running
    return 0;               // ANSI C expects this
}
