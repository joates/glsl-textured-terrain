/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 018

#ifndef LUTIL_H
#define LUTIL_H

#include "LOpenGL.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

//Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;
const int SCREEN_FPS = 60;

bool initGL();
/*
Pre Condition:
 -A valid OpenGL context
Post Condition:
 -Initializes clear color
 -Enables blending
 -Disables depth testing
 -Initializes GLEW
 -Initializes DevIL and DevIL utilities
 -Reports to console if there was an OpenGL error
 -Reports to console if there was a GLEW error
 -Reports to console if there was a DevIL error
 -Returns false if there was an error in initialization
Side Effects:
 -Sets viewport to the fill rendering area
 -Clear color is set to black
 -Texturing is enabled
 -Sets DevIL clear color to transparent white
*/

bool loadGP();
/*
Pre Condition:
 -A valid OpenGL context
Post Condition:
 -Loads graphics programs
 -Reports to console if there was an error in loading the graphics programs
 -Returns true if the graphics programs load successfully
Side Effects:
 -Binds textured polygon program
*/

bool loadMedia();
/*
Pre Condition:
 -A valid OpenGL context
Post Condition:
 -Loads media to use in the program
 -Reports to console if there was an error in loading the media
 -Returns true if the media loaded successfully
Side Effects:
 -None
*/

void scan_keys( unsigned char key, int x, int y );
void pressKey( int key, int x, int y );
void releaseKey( int key, int x, int y );

void update();
/*
Pre Condition:
 -None
Post Condition:
 -Does per frame logic
Side Effects:
 -None
*/

void render();
/*
Pre Condition:
 -A valid OpenGL context
 -Active modelview matrix
Post Condition:
 -Renders the scene
Side Effects:
 -Clears the color buffer
 -Swaps the front/back buffer
*/

#endif
