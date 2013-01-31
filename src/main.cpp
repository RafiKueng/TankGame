/**
 * main.cpp
 *
 * Author   :   Fatih Erol
 * Date     :   16.02.2011
 *
 * All rights reserved.
 */

// Includes
#include "exception.hpp"
#include "scene.hpp"
#include "gl_includes.hpp"
GRAPHICSLAB_NAMESPACE_USE

#include <iostream>

//////////////////////////////////////////////////////////////////////////////
// Signal handler
//////////////////////////////////////////////////////////////////////////////
#include <csignal>
#include <cstdlib>
void signalHandler(int signal)
{
    std::cout << "Caught signal " << signal << "... Exiting..." << std::endl;
    exit( EXIT_SUCCESS );
}
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Global variable and functions for binding GLUT callbacks
//////////////////////////////////////////////////////////////////////////////

static Scene* scene;

static void __displayFunc()
{
    scene->onPaint();
}

static void __reshapeFunc( int width, int height )
{
    scene->onResize( width, height );
}

static void __keyboardFunc( unsigned char key, int x, int y )
{
    scene->onKey( key );
}

static void __keyboardUpFunc( unsigned char key, int x, int y )
{
    scene->onKeyUp( key );
}

static void __specialFunc( int key, int x, int y )
{
    scene->onSpecialKey( key );
}

static void __specialUpFunc( int key, int x, int y )
{
    scene->onSpecialKeyUp( key );
}

static void __entryFunc( int state )
{
    scene->onMouseEntry( state );
}

static void __mouseFunc( int button, int state, int x, int y )
{
    scene->onMouseClick( button, state, x, y );
}

static void __motionFunc( int x, int y )
{
    scene->onMouseMove( x, y );
}

static void __passiveMotionFunc( int x, int y )
{
    scene->onMousePassiveMove( x, y );
}

static void __visibilityFunc( int state )
{
    scene->onVisible( state );
}


static void __timerFunc( int value )
{
    scene->onTimer( value );
    // To have max 30 frames per second,
    // we schedule to run this function every 1000/33'th millisecond.
    static const int MAX_FPS = 30;
    static const int MILLIS_PER_FRAME = 1000/MAX_FPS;
    glutTimerFunc( MILLIS_PER_FRAME, __timerFunc, MILLIS_PER_FRAME );
}


static void __idleFunc()
{
    scene->onIdle();
}


//////////////////////////////////////////////////////////////////////////////
// Main function
//////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[] )
{
    // Signal handler
	signal(SIGINT, signalHandler);
    
    try
    {
		// initialize sound
		Sound::initialize();

		// Initialize GLUT
        glutInit( &argc, argv );
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
        glutInitWindowSize( 1024, 768 );
        glutInitWindowPosition( -1, -1 );
        
        // Create GLUT Window
        int windowID = glutCreateWindow( "Tank Game" );    
		
        // Bind handlers
        glutDisplayFunc( __displayFunc );
        glutReshapeFunc( __reshapeFunc );
        glutKeyboardFunc( __keyboardFunc );
		glutKeyboardUpFunc( __keyboardUpFunc );
        glutSpecialFunc( __specialFunc );
		glutSpecialUpFunc( __specialUpFunc );
        glutMouseFunc( __mouseFunc );
        glutMotionFunc( __motionFunc );
        glutPassiveMotionFunc( __passiveMotionFunc );
        glutEntryFunc( __entryFunc );
        glutVisibilityFunc( __visibilityFunc );
        glutTimerFunc( 1, __timerFunc, 1 );
//        glutIdleFunc( __idleFunc );
        
        // Show window and start event loop
		scene = new Scene();
        scene->initialize();
        glutMainLoop();
    }
    catch ( const Exception &e )
    {
        std::cerr << "Exception : " << e.what() << std::endl;
        return 1;
    }
    catch ( ... )
    {
        std::cerr << "Exception : Unknown exception" << std::endl;
        return 1;
    }

    delete scene;

    return 0;
};

