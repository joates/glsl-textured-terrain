/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 034

#include "LUtil.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "LTexturedPolygonProgram2D.h"
#include "LTexture.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//Textured polygon shader
LTexturedPolygonProgram2D gTexturedPolygonProgram2D;

//Loaded texture
LTexture gOpenGLTexture;
LColorRGBA gTextureColor = { 1.f, 1.f, 1.f, 0.75f };

//GlobalTime
timespec ts;
double start_time, elapsed_time;

bool initGL()
{
    // clock_gettime(CLOCK_MONOTONIC, &ts); // Works on FreeBSD
    clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux
    start_time = ts.tv_sec;

    //Initialize GLEW
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return false;
    }

    //Make sure OpenGL 2.1 is supported
    if( !GLEW_VERSION_2_1 )
    {
        printf( "OpenGL 2.1 not supported!\n" );
        return false;
    }

    //Set the viewport
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Set blending
    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    //Initialize DevIL and DevILU
    ilInit();
    iluInit();
    ilClearColour( 255, 255, 255, 000 );

    //Check for error
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }

    return true;
}

bool loadGP()
{
	//Load textured shader program
	if( !gTexturedPolygonProgram2D.loadProgram() )
	{
		printf( "Unable to load textured polygon program!\n" );
		return false;
	}

	//Bind textured shader program
	gTexturedPolygonProgram2D.bind();

	//Initialize projection
	gTexturedPolygonProgram2D.setProjection( glm::ortho<GLfloat>( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 ) );
	gTexturedPolygonProgram2D.updateProjection();

	//Initialize modelview
	gTexturedPolygonProgram2D.setModelView( glm::mat4() );
	gTexturedPolygonProgram2D.updateModelView();

    //Set texture unit
    gTexturedPolygonProgram2D.setTextureUnit( 0 );

	//Set program for texture
	LTexture::setTexturedPolygonProgram2D( &gTexturedPolygonProgram2D );

	return true;
}

bool loadMedia()
{
	//Load texture
	if( !gOpenGLTexture.loadTextureFromFile32( "img/demo.png" ) )
	{
	    printf( "Unable to load texture!\n" );
		return false;
	}

    return true;
}

void update()
{
    //Screen resolution
    gTexturedPolygonProgram2D.setResolution( (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT );

    //Elapsed time (rtc millisecond precision)
    clock_gettime(CLOCK_REALTIME, &ts);
    ldiv_t ldivresult = ldiv(ts.tv_nsec, 1000000);
    elapsed_time = ts.tv_sec - start_time + ((double)ldivresult.quot / 1000);

    gTexturedPolygonProgram2D.setGlobalTime( (float)elapsed_time );
}

void scan_keys( unsigned char key, int x, int y )
{
    //ESC key to exit
    if (key == 27) {
        glutLeaveGameMode();
        glutLeaveMainLoop();
    }
}

void render()
{
    /*
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

	//Reset transformations
	gTexturedPolygonProgram2D.setModelView( glm::mat4() );

	//Render texture centered
	gTexturedPolygonProgram2D.setTextureColor( gTextureColor );
	gOpenGLTexture.render( ( SCREEN_WIDTH - gOpenGLTexture.imageWidth() ) / 2.f, ( SCREEN_HEIGHT - gOpenGLTexture.imageHeight() ) / 2.f );

    //Update screen
    glutSwapBuffers();
    */

    GLfloat half_width  = SCREEN_WIDTH  / 2.f;
    GLfloat half_height = SCREEN_HEIGHT / 2.f;

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset transformations
    glLoadIdentity();

    //Screen quad
    glTranslatef( half_width, half_height, 0.f );
    glBegin( GL_QUADS );
        glColor3f( 1.f, 1.f, 1.f );
        glVertex2f( -half_width, -half_height );
        glVertex2f(  half_width, -half_height );
        glVertex2f(  half_width,  half_height );
        glVertex2f( -half_width,  half_height );
    glEnd();

    //Rendering a texture loads it to GL_TEXTURE0 + texID
    gOpenGLTexture.render( 0.0, 0.0 );

    //Update screen
    glutSwapBuffers();
}
