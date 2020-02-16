//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.
// ******Rotation applied in the shader
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;


const int n = 300;
const int circleRadius = 100;
const int windowHeight = 500;
const int windowWidth = 500;
const int height = 40;
const int width = 10;
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

GLfloat mouseX = 0.0;
GLfloat mouseY = 0.0;
GLfloat radius = 0.0;


bool isAnimationMode = 0;


float localMouseX = 0.0;
float localMouseY = 0.0;

point4 Lshape[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -1.0, -1.0,  0.0, 1.0 ),
    point4( -1.0, -0.6,  0.0, 1.0 ),
    point4( -0.9, -0.6,  0.0, 1.0 ),
    point4( -0.9, -1.0,  0.0, 1.0 ),
    point4( -0.9, -1.0,  0.0, 1.0 ),
    point4( -0.9, -0.9,  0.0, 1.0 ),
    point4( -0.8, -0.9,  0.0, 1.0 ),
    point4( -0.8, -1.0,  0.0, 1.0 ) 
};

// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan */
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  theta;  // The location of the "theta" shader uniform variable

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; Lshape[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; Lshape[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; Lshape[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; Lshape[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; Lshape[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; Lshape[Index] = vertices[d]; Index++;
}

void
createPoints(float x,float y){
    
    float h = height*0.01;
    float w = width*0.01;
    float refX = x-1.0+w;
    float refY = y-1.0;
    Lshape[0] = point4( refX, refY,  0.0, 1.0 );//3
    Lshape[1] = point4( refX-w, refY,  0.0, 1.0 );//0
    Lshape[2] = point4( refX-w, refY+h,  0.0, 1.0 );//1    
    
    Lshape[3] = point4( refX, refY,  0.0, 1.0 );//3
    Lshape[4] = point4( refX-w,refY+h,  0.0, 1.0 );//1
    Lshape[5] = point4( refX, refY+h,  0.0, 1.0 );//2

    Lshape[6] = point4( refX, refY,  0.0, 1.0 );//3
    Lshape[7] = point4( refX, refY+w,  0.0, 1.0 );//4
    Lshape[8] = point4( refX+w, refY+w,  0.0, 1.0 );//5

    Lshape[9] = point4( refX, refY,  0.0, 1.0 );//3
    Lshape[10] = point4( refX+w, refY,  0.0, 1.0 );//6
    Lshape[11] = point4( refX+w, refY+w,  0.0, 1.0 );//5

}


//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 ); 
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    createPoints(0.0,0.0);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(Lshape) + sizeof(colors),NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Lshape), Lshape );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(Lshape), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader36.glsl", "fshader36.glsl" );
    //GLuint program = InitShader( "vs.glsl", "fs.glsl" );
    
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(Lshape)) );

    theta = glGetUniformLocation( program, "theta" );
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}
void
drawLAt(float x, float y, float angle){
    createPoints(x-width*0.01,y);
    if(isAnimationMode){
        360.0/n;
        localMouseX = 0.0;
        localMouseY = 0.0;
    }
    else{
        Theta[Axis] = 30.0;
        localMouseX = (x)-1.0;
        localMouseY = y-1.0;
        //createPoints(x-width*0.01,y);
        printf("%f\n",x);
        printf("%f\n",y);
    }
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(Lshape) + sizeof(colors),NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Lshape), Lshape );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(Lshape), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader36.glsl", "fshader36.glsl" );
    //GLuint program = InitShader( "vs.glsl", "fs.glsl" );
    
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(Lshape)) );

    theta = glGetUniformLocation( program, "theta" );
    radius = glGetUniformLocation( program, "radius" );

    mouseX = glGetUniformLocation( program, "mouseX" );
    mouseY = glGetUniformLocation( program, "mouseY" );

    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}
//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    float radiusY = 0.0;
    if(isAnimationMode){
        radiusY = (circleRadius*1.0)/(windowHeight*1.0)*2;
    }
    
    
    //printf("%f\n",radiusY);

    glUniform1f(radius,radiusY);
    glUniform1f(mouseX, localMouseX);
    glUniform1f(mouseY, localMouseY);

    glUniform3fv( theta, 1, Theta );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    glutSwapBuffers();
}

//----------------------------------------------------------------------------


void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    
    case 'r': 
	    Theta[Axis] += 5.0;
	    break;
    
    case 'a': 
        drawLAt(1.0,1.0,0.0);  
	    isAnimationMode = 1;
	    break;
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {

	    //case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
	    //case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
	    case GLUT_RIGHT_BUTTON:   
            //printf("%d\n",x);
            //printf("%d\n",y);
            float mousePositionX = (x*1.0)/(windowWidth)*2.0;
            //printf("%f\n",mousePositionX);

            float mousePositionY = 2.0-((y*1.0)/(windowWidth)*2.0);
            //printf("%f\n",mousePositionY);

            drawLAt(mousePositionX,mousePositionY,30.0);  
            break;
	}
    }
}

//----------------------------------------------------------------------------

void
idle( void )
{
    if(isAnimationMode){
        Theta[Axis] += 360.0/n;
    }
    
    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
#ifdef __APPLE__ 
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
#else
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
#endif
    glutInitWindowSize(windowWidth,windowHeight);
//    glutInitContextVersion( 3, 1 );
//    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );
	glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
