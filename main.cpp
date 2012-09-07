/*
// Created by Ethan Sherbondy
// 6.837, Fall 2012
//
// References: 
// http://alumni.cs.ucsb.edu/~wombatty/tutorials/opengl_mac_osx.html
// http://www.songho.ca/opengl/gl_transform.html
*/

#ifdef __APPLE__
# include <GLUT/glut.h>
// Just in case we need these later
// # include <OpenGL/gl.h>
// # include <OpenGL/glu.h>
#else
# include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <math.h> // for fmod
#include <vecmath.h>
using namespace std;

// Globals

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes
// globals are evil!
namespace g
{
    typedef map<char, float> color_map;
    static color_map create_colors()
    {
        color_map colors;
        colors['r'] = 0.0; colors['g'] = 0.0, colors['b'] = 0.0;
        return colors;
    }

    // man, I should be using C++ 11 :(
    color_map colors = create_colors();
    int       colorN = 0;
    char      lastKey = 'c';
    float     lx, ly; // light position
    int       xPos, rotationX = 0;
}


// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }

// now I can mod all of the number types without complaint
inline int fmod(int a, int b)
{ return a % b; }

// I'm just practicing my C++. This is excessive.
template <class T>
void inc_mod(T &a, T increment, T max) {
    a = fmod((a + increment), max);
}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
		inc_mod(g::colorN, 1, 3);
        g::lastKey = key;
        break;
    case 'r':
    case 'g':
    case 'b':
        inc_mod(g::colors[key], 0.1f, 1.0f);
        g::lastKey = key;
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
		g::ly += 1.0f;
		break;
    case GLUT_KEY_DOWN:
		g::ly -= 1.0f;
		break;
    case GLUT_KEY_LEFT:
		g::lx -= 1.0f;
		break;
    case GLUT_KEY_RIGHT:
		g::lx += 1.0f;
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

    glRotatef(g::rotationX, 0, 1, 0);

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };

    GLfloat customColor[4] = {g::colors['r'], g::colors['g'], g::colors['b'], 1.0};
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
                (g::lastKey == 'c' ? diffColors[g::colorN] : customColor));

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {g::lx, g::ly, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
	glutSolidTeapot(1.0);
    
    // Dump the image to the screen.
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void motionFunc(int x, int y)
{
    // move left or right based on previous x position.
    inc_mod(g::rotationX, x - g::xPos, 360);
    g::xPos = x;
    glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        g::xPos = x;
    }
    glutPostRedisplay();
}

void loadInput()
{
	// load the OBJ file here
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys
    glutMotionFunc(motionFunc);
    glutMouseFunc(mouseFunc);

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
