#ifndef KEYBOARD_MOUSE_H_
#define KEYBOARD_MOUSE_H_

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <maths_funcs.h> //Anton's math class

// C++ standard libraries
#include <fstream>
#include <iostream>

// Extern variables called in main.cpp
extern const int height;
extern const int width;

// Second viewport toggle switch
extern bool model2ToggleView;

// Camera Position
extern float x_mouse;
extern float y_mouse;
extern float z_mouse;

// Model Position X, Z
extern float x_pos;
extern float y_pos;
extern float z_pos;

// Light Intensity
extern float ambientStrength;
extern float specularStrength;

// Rotation toggle
extern bool modelRotationToggle;
extern float modelRotation;

// Light Position
extern vec3 lightPositionDirection;
extern vec3 lightPosition;
extern bool lightPositionToggle;

// keyboard/mouse input function declarations
void keyPress(unsigned char key, int xmouse, int ymouse);

void mouseMove(int x, int y);

void mouseWheel(int key, int wheeldir, int x, int y);

void specialKeypress(int key, int x, int y);

#endif //KEYBOARD_MOUSE_H_