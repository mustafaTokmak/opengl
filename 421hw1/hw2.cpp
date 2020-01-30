#include <stdlib.h>			// standard definitions

#include <stdio.h>			// C I/O (for sprintf) 
#include <math.h>			// standard definitions
#include <time.h>
#include <unistd.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// global variable declarations
//Settings
// iki araç arasındaki minimum mesafe
int vehicle_interval_factor = 1;
// her adimda araçların gideceği mesafe
int motion_precise = 10; 
// yeni araç oluştrulma frekansi 
int vehicle_freq = 50;

//yeni coin oluşturulma frekansi
int coin_freq = 2;



int ispause = 0;
int isGameOver = 0;
GLint windowHeight=600, windowWidth=500;
const int totalNumberOfLane = 24;
GLint totalLane = totalNumberOfLane+1;
GLdouble agentX = 0.5;
GLint agentRoad = 0;
GLint agentLane = 0;
GLint agentDirection = 1;
GLint timerDifficulty = 50;
GLint score = 0;
GLint totalCoin = 0;
// all odd lane is left to right
struct lane
{
    int road;
    int laneNo;
    int velocity; // -5,5
};
struct coin
{
    int road;
    int lane;
    double xAxis;
};

struct vehicle 
{  
   //car type is 0 
   //truck type is 1 
   int type;
   lane laneRef;
   double xAxis;

};
const int numberOfCoin = totalNumberOfLane;
struct coin coinArr[numberOfCoin];
int coinIndex = 0;
// lanes + sidewalks
struct lane laneArr[totalNumberOfLane];
int totalVehicle = totalNumberOfLane*25;
struct vehicle vehicleArr[totalNumberOfLane*25];

// her bir lanedeki son aracin vehicleArrdaki indexi
int tailVehicleIndexArr[totalNumberOfLane];

//son oluşturulan vehicle'in
int vehicleIndex = 0;

void myInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);		
    glShadeModel(GL_FLAT);	
}

double getLaneSize(){
    double sidewalks_size = 1/(windowHeight/(1.0*totalLane));
    return sidewalks_size;
}
void myReshape(int winWidth, int winHeight) 
{
    windowHeight = winHeight;
    windowWidth = winWidth;
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
    glColor3f(1.0, 0.0, 0.0);       // set color to red
    glBegin(GL_POLYGON);
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
    glColor3f(0.0, 1.0, 0.0);       // set color to red
    if(type == 1){
        vehicleWidth = vehicleWidth*2;
        glColor3f(0.0, 0.0, 1.0);       // set color to red
    }
    double y = calculateVehicleYaxis(road,lane);
    

    glBegin(GL_POLYGON);           
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
void drawCoins()
{
    for(int i=0;i < numberOfCoin;i++){
        if(coinArr[i].xAxis != -1.0){
            drawCoin(coinArr[i].road,coinArr[i].lane,coinArr[i].xAxis);
        }
    }
}
void initCoins()
{   
    struct coin coinTemp;
    coinTemp.xAxis = -1.0;
    for(int i=0; i<numberOfCoin;i++){
        coinArr[i] = coinTemp;
    }
}
void printScore()
{
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(0.01, 0.01);
    
    char scoreStr[20];
    sprintf(scoreStr,"score : %d",score);
    char *c;
    for (c=scoreStr; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glRasterPos2f(0.01, 0.97);
    char coinStr[10];
    sprintf(coinStr,"coin : %d",totalCoin);
    char *d;
    for (d=coinStr; *d != '\0'; d++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *d);
    }
}

void GameOver(){
    ispause = 1;
    isGameOver = 1;
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(0.35, 0.965);
    char *gameover = "GAME OVER";
    char *c;
    for (c=gameover; *c != '\0'; c++) {
        
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}
void createCoin()
{
    int prob = rand() % (20 -coin_freq);
    if (prob != 0)
        return;
    struct coin coinTemp;
    double coinXaxis = ((rand() % 500)*1.0) / 500;
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3}; 
    int coinRoad = (rand() % 5) + 1;
    int coinLane;
    if( coinRoad != 0 ){
        coinLane = (rand() % (numberOfLaneOfRoads[coinRoad] - 1)) + 1;
    } 
    coinTemp.lane = coinLane;
    coinTemp.road = coinRoad;
    coinTemp.xAxis = coinXaxis;
    coinArr[coinIndex] = coinTemp;
    coinIndex = (coinIndex + 1) % numberOfCoin;
}
int isVehicleAgentCollision()
{   
    double vehicleHeight  = getLaneSize()*0.8;
    double vehicleWidth = vehicleHeight;

    // for all vehicles
    for(int i=0;i<totalNumberOfLane*25;i++){
        if (vehicleArr[i].type != -1){
            
            int road = vehicleArr[i].laneRef.road;
            int lane = vehicleArr[i].laneRef.laneNo;
            if (agentRoad == road && agentLane == lane){
                if(vehicleArr[i].type == 0){
                    if (abs(agentX - vehicleArr[i].xAxis ) < vehicleWidth){
                        GameOver();
                        // pause game
                        // show score
                    }
                }
                else if(vehicleArr[i].type == 1){
                    if (abs(agentX - vehicleArr[i].xAxis ) < vehicleWidth*1.5){
                        GameOver();
                        // pause game
                        // show score
                    }
                }
                
            }
        }     
    }
    return 0;

}


int isCoinAgentCollision()
{   double laneSize = getLaneSize();
    for(int i=0;i < numberOfCoin;i++){
        if(coinArr[i].xAxis != -1.0){
            if(coinArr[i].lane == agentLane && coinArr[i].road == agentRoad){
                if(abs(coinArr[i].xAxis - agentX) < laneSize*0.8){
                    score = score + 5; // score for each 
                    totalCoin = totalCoin + 1;
                    coinArr[i].xAxis = -1.0;
                }
            }
        }
    }
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
int getLaneIndex(int road,int lane)
{
    int index = 0;
    int laneV; // from -5 to 5 = (rand() % 6 ) -10;
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};
    
    for(int i =0;i < 6; i++){
       
        for(int j = 0; j < numberOfLaneOfRoads[i]+1; j++){
            if (i == road && j == lane){
                return index;
            }
            index += 1;
        }
    }

}

void addVehicle(struct lane lane,int random){
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
    if ( random == 1){
        vehicleTemp.xAxis = ((rand() % 10)*1.0)/10;
    }
    vehicleArr[vehicleIndex] = vehicleTemp;
    tailVehicleIndexArr[roadNo*4+laneNo] = vehicleIndex;
    vehicleIndex = (vehicleIndex + 1) % totalVehicle;
    
}

void initvehicles() 
{   
    timerDifficulty = 1;
    
    for(int i =0; i < totalNumberOfLane*25;i++){
        vehicleArr[i].type=-1;
    }
    for(int i=0;i<totalNumberOfLane;i++){
        struct lane l = laneArr[i];
        addVehicle(l,1);
    }
    for(int i=0;i<totalNumberOfLane;i++){
        struct lane l = laneArr[i];
        addVehicle(l,1);
    }
    for(int i=0;i<totalNumberOfLane;i++){
        struct lane l = laneArr[i];
        addVehicle(l,1);
    }
    for(int i=0;i<totalNumberOfLane*25;i++){
        if (vehicleArr[i].type != -1){
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
int isCreateVehicleAvaliable(int road,int lane,int type)
{   
    int prob = rand () % (101-vehicle_freq);
    if (prob != 0)
        return 0;
    double laneSize = getLaneSize();
    double vehicleHeight = laneSize*0.8;
    double vehicleWidth = vehicleHeight*((1.0*windowHeight)/(1.0*windowWidth));
    
    
    int vehicleIndex = tailVehicleIndexArr[road*4+lane];
    struct vehicle tailVehicle = vehicleArr[vehicleIndex];
    double interval = vehicleWidth *(vehicle_interval_factor+ (type * 0.5) + (tailVehicle.type * 0.5));
    double x =  tailVehicle.xAxis;
    int vel = tailVehicle.laneRef.velocity;
    fflush(stdout);
    if (vel < 0 && abs(1.1 - x) > interval){
        return 1;
    }
    else if (vel > 0 && abs(-0.1 - x) > interval){
        return 1;
    }
    else{
        return 0;
    }
}
void createNewVehicle()
{
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};  
    int road = ( rand() % 5 ) + 1;
    int laneNo = (rand() % (numberOfLaneOfRoads[road])) + 1;
    int type = rand() % 2;
    int res = isCreateVehicleAvaliable(road,laneNo,type);
    if(res == 1){
        int i = getLaneIndex(road,laneNo);
        addVehicle(laneArr[i],0);
    }
}

void updateVehicles(){
    double laneSize = getLaneSize();
    double vehicleHeight = laneSize*0.8;
    double vehicleWidth = vehicleHeight*((1.0*windowHeight)/(1.0*windowWidth));
    double baseV = 1/((1.0*windowWidth)/vehicleWidth)*motion_precise;
    
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
    isCoinAgentCollision();
    drawAgent(agentRoad,agentLane,agentX,agentDirection);
    drawCoins();
    printScore();
    if (isGameOver){
        GameOver();
    }
    glFlush();				
    glutSwapBuffers();		
    isVehicleAgentCollision();

}

void myTimeOut(int id) {

	glutPostRedisplay();			 
	glutTimerFunc(2000, myTimeOut, 0);  
}
static void Timer(int value){
    if(! ispause){
        updateVehicles();
        createCoin();
        createNewVehicle();
        
        glutPostRedisplay();
        // 100 milliseconds
        glutTimerFunc(timerDifficulty, Timer, 0);
    }
}

void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        
        case 'q':
		    exit(0);	
            break;
        case 'w':
            coin_freq = 1;
            break;
        case 'e':
            coin_freq = 2;
            break;
        case 'r':
            coin_freq = 4;
            break;
        case 't':
            coin_freq = 6;
            break;
        case 'y':
            coin_freq = 8;
            break;
        case 'u':
            coin_freq = 10;
            break;

        case '1':
            vehicle_freq = 20;
            break;
        case '2':
            vehicle_freq = 30;
            break;
        case '3':
            vehicle_freq = 40;
            break;
        case '4':
            vehicle_freq = 50;
            break;
        case '5':
            vehicle_freq = 60;
            break;
        case '6':
            vehicle_freq = 70;
            break;
        case '7':
            vehicle_freq = 80;
            break;
        case '8':
            vehicle_freq = 90;
            break;
        case '9':
            vehicle_freq = 100;
            break;
	}

    glutPostRedisplay();		// request redisplay
}
void updateOneStep(){
    updateVehicles();
    createCoin();
    createNewVehicle();
    
    glutPostRedisplay();

}

void myMouse(int b, int s, int x, int y) {
	switch  ( b ) {    // b indicates the button
	case GLUT_LEFT_BUTTON:
		if (s == GLUT_DOWN)      // button pressed
			updateOneStep();//glutTimerFunc(NULL);
		else if (s == GLUT_UP)   // button released
			;
		break;
		//... other button events may follow
	
    case GLUT_RIGHT_BUTTON:
		if (s == GLUT_DOWN)      // button pressed
			ispause = 1;//glutTimerFunc(NULL);
		break;
		//... other button events may follow
	}
    
}

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
                       
                    }
                }
                 score += 1;
            }

        }
        else{
            GameOver();
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
            score += 1;
        }
        else{
            GameOver();
        }
    }
    
    
}
void catchKey(int key, int x, int y)
{   
    int numberOfLaneOfRoads[] = {0,4,3,4,4,3};
    int laneDirection = rand() % 5;
    if(!isGameOver){
        updateAgent(key);
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{   
    srand ( time(NULL) );
    glutInit(&argc, argv);   //Initialize glut and gl
    glutInitDisplayMode(		
		GLUT_DOUBLE |		// double buffering
		GLUT_RGB );		    // RGB color mode

    glutInitWindowSize(windowWidth,windowHeight);  
    glutInitWindowPosition(0,0);    // upper left
    glutCreateWindow("My First OpenGL Program"); 	// create the window

    glutDisplayFunc(myDisplay);		//  call myDisplay to redraw window
    glutKeyboardFunc(myKeyboard);   // call myKeyboard when key is hit
    glutReshapeFunc(myReshape);     // call myReshape if window is resized
    glutMouseFunc(myMouse);         // call in mouse event 
    Timer(0);
    myInit();				// our own initializations
    glutSpecialFunc(catchKey);
    initLanes();
    initvehicles();
    initCoins();
    glutMainLoop();			// pass control to GLUT, start it running
    return 0;               // ANSI C expects this
}
