#ifndef KEYBOARD_MOUSE_H_
#define KEYBOARD_MOUSE_H_

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>

// C++ standard libraries
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
extern float z_pos;

// Secondary Model RotationDegree Y
extern float leftModelXDeg;
extern float rightModelXDeg;
extern float leftModelYDeg;
extern float rightModelYDeg;

// Light Intensity
extern float ambientStrength;
extern float specularStrength;

// keyboard/mouse input function declarations
void keyPress(unsigned char key, int xmouse, int ymouse);

void mouseMove(int x, int y);

void mouseWheel(int key, int wheeldir, int x, int y);

void specialKeypress(int key, int x, int y);

#endif //KEYBOARD_MOUSE_H_