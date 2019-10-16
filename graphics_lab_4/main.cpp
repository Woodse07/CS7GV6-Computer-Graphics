
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include "maths_funcs.h"
#include "teapot.h" // teapot mesh

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define debug_mode true
using namespace std;
GLuint shaderProgramID;

unsigned int teapot_vao = 0;
int width = 800;
int height = 600;

GLuint loc1;
GLuint loc2;
// Movement Variables
vec3 body_translation = vec3(0, 0, 0);
// Head
GLfloat head_nod = 0;
bool head_direction_front_back = true;
// Left Leg
GLfloat l_thigh_angle = 0;
bool l_thigh_direction_front_back = true;
GLfloat l_foot_angle = 0;
bool l_foot_direction_front_back = true;
// Right Leg
GLfloat r_thigh_angle = 0;
bool r_thigh_direction_front_back = true;
GLfloat r_foot_angle = 0;
bool r_foot_direction_front_back = true;

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS

// Create a NULL-terminated string by reading the provided file
char* readShaderSource(const char* shaderFile) {
	FILE* fp = fopen(shaderFile, "rb"); //!->Why does binary flag "RB" work and not "R"... wierd msvc thing?

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "../Shaders/simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "../Shaders/simpleFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS

void generateObjectBufferTeapot() {
	GLuint vp_vbo = 0;

	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normals");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_vertex_points, GL_STATIC_DRAW);
	GLuint vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_normals, GL_STATIC_DRAW);

	glGenVertexArrays(1, &teapot_vao);
	glBindVertexArray(teapot_vao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}


#pragma endregion VBO_FUNCTIONS


void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
	int ortho_mat_location = glGetUniformLocation(shaderProgramID, "ortho");

	// Hierarchy of Teapots

	// Root of the Hierarchy
	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0, (float)width / (float)height, 0.1, 100.0);
	mat4 ortho = identity_mat4();
	mat4 body = identity_mat4();
	body = rotate_z_deg(body, 0.0f);
	body = rotate_y_deg(body, 0.0f);
	body = rotate_x_deg(body, 10.0f);
	body = translate(body, vec3(0.0f, 5.0f, -60.0f));
	body = scale(body, vec3(0.7f, 0.7f, 0.7f));
	body = translate(body, body_translation);

	// for the body/root, we orient it in global space
	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, body.m);
	glUniformMatrix4fv(ortho_mat_location, 1, GL_FALSE, ortho.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	// child of hierarchy
	// Teapot Man Head
	mat4 head = identity_mat4();
	head = scale(head, vec3(0.28f, 0.28f, 0.28f));
	head = translate(head, vec3(-0.63f, 10.2f, 0.0f));
	head = rotate_x_deg(head, head_nod);
	mat4 head_body = body * head;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, head_body.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	// Teapot Man Right Thigh
	mat4 r_thigh = identity_mat4();
	r_thigh = scale(r_thigh, vec3(0.33f, 0.6f, 0.33f));
	r_thigh = translate(r_thigh, vec3(5.5f, -12.f, 0.0f));
	r_thigh = rotate_x_deg(r_thigh, r_thigh_angle);
	r_thigh = body * r_thigh;
	// update uniform & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, r_thigh.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	// Teapot Man Right Foot
	mat4 r_foot = identity_mat4();
	r_foot = scale(r_foot, vec3(1.2121f, 0.55f, 1.2121f));
	r_foot = translate(r_foot, vec3(0.0f, -12.0f, 0.0f));
	r_foot = rotate_x_deg(r_foot, r_foot_angle);
	r_foot = r_thigh * r_foot;
	// update uniform & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, r_foot.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	// Teapot Man Left Thigh
	mat4 l_thigh = identity_mat4();
	l_thigh = scale(l_thigh, vec3(0.33f, 0.6f, 0.33f));
	l_thigh = translate(l_thigh, vec3(-7.0f, -12.f, 0.0f));
	l_thigh = rotate_x_deg(l_thigh, l_thigh_angle);
	l_thigh = body * l_thigh;
	// update uniform & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, l_thigh.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	// Teapot Man Left Foot
	mat4 l_foot = identity_mat4();
	l_foot = scale(l_foot, vec3(1.2121f, 0.55f, 1.2121f));
	l_foot = translate(l_foot, vec3(0.0f, -12.0f, 0.0f));
	l_foot = rotate_x_deg(l_foot, l_foot_angle);
	l_foot = l_thigh * l_foot;
	// update uniform & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, l_foot.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	glutSwapBuffers();
}



void updateScene() {

	// Placeholder code, if you want to work with framerate
	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;

///	rotatez += 0.2f;
	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();
	// load teapot mesh into a vertex buffer array
	generateObjectBufferTeapot();

}

// Placeholder code for the keypress
void keypress(unsigned char key, int xmouse, int ymouse) {

	switch (key) {
		// General Teapot man translation
		// Forward
		case('w'):
			body_translation.v[2] = body_translation.v[2] - 0.2f;
			break;
		// Backwards
		case('s'):
			body_translation.v[2] = body_translation.v[2] + 0.2f;
			break;
		// Right
		case('a'):
			body_translation.v[0] = body_translation.v[0] - 0.2f;
			break;
		// Left
		case('d'):
			body_translation.v[0] = body_translation.v[0] + 0.2f;
			break;
		// Head Nod
		case('h'):
			std::cout << (debug_mode ? "Nodding" : "") << std::endl;
			if (head_direction_front_back == true) {
				std::cout << (debug_mode ? "Nodding front +5" : "") << std::endl;
				head_nod += 5;
			}else if (head_direction_front_back == false) {
				std::cout << (debug_mode ? "Nodding back -5" : "") << std::endl;
				head_nod -= 5;
			}
			if (head_nod >= 35) {
				std::cout << (debug_mode ? "Nodding back" : "") << std::endl;
				head_direction_front_back = false;
			}else if (head_nod <= -35) {
				std::cout << (debug_mode ? "Nodding front" : "") << std::endl;
				head_direction_front_back = true;
			}
			break;
		// left thigh movement
		case('q'):
			std::cout << (debug_mode ? "Left thighing" : "") << std::endl;
			if (l_thigh_direction_front_back == true) {
				std::cout << (debug_mode ? "Left thighing +5" : "") << std::endl;
				l_thigh_angle += 5;
			}
			else if (l_thigh_direction_front_back == false) {
				std::cout << (debug_mode ? "Left thighing -5" : "") << std::endl;
				l_thigh_angle -= 5;
			}
			if (l_thigh_angle >= 35) {
				std::cout << (debug_mode ? "Left thighing back" : "") << std::endl;
				l_thigh_direction_front_back = false;
			}
			else if (l_thigh_angle <= -35) {
				std::cout << (debug_mode ? "Left thighing front" : "") << std::endl;
				l_thigh_direction_front_back = true;
			}
			break;
		case('z'):
			std::cout << (debug_mode ? "Left Footing" : "") << std::endl;
			if (l_foot_direction_front_back == true) {
				std::cout << (debug_mode ? "Left footing +3" : "") << std::endl;
				l_foot_angle += 5;
			}
			else if (l_foot_direction_front_back == false) {
				std::cout << (debug_mode ? "Left footing -3" : "") << std::endl;
				l_foot_angle -= 5;
			}
			if (l_foot_angle >= 10) {
				std::cout << (debug_mode ? "Left footing back" : "") << std::endl;
				l_foot_direction_front_back = false;
			}
			else if (l_foot_angle <= -25) {
				std::cout << (debug_mode ? "Left footing front" : "") << std::endl;
				l_foot_direction_front_back = true;
			}
			break;
		// right thigh movement
		case('e'):
			std::cout << (debug_mode ? "Right thighing" : "") << std::endl;
			if (r_thigh_direction_front_back == true) {
				std::cout << (debug_mode ? "Right thighing +5" : "") << std::endl;
				r_thigh_angle += 5;
			}
			else if (r_thigh_direction_front_back == false) {
				std::cout << (debug_mode ? "Right thighing -5" : "") << std::endl;
				r_thigh_angle -= 5;
			}
			if (r_thigh_angle >= 35) {
				std::cout << (debug_mode ? "Right thighing back" : "") << std::endl;
				r_thigh_direction_front_back = false;
			}
			else if (r_thigh_angle <= -35) {
				std::cout << (debug_mode ? "Right thighing front" : "") << std::endl;
				r_thigh_direction_front_back = true;
			}
			break;
		case('c'):
			std::cout << (debug_mode ? "Right Footing" : "") << std::endl;
			if (r_foot_direction_front_back == true) {
				std::cout << (debug_mode ? "Right footing +3" : "") << std::endl;
				r_foot_angle += 5;
			}
			else if (r_foot_direction_front_back == false) {
				std::cout << (debug_mode ? "Right footing -3" : "") << std::endl;
				r_foot_angle -= 5;
			}
			if (r_foot_angle >= 10) {
				std::cout << (debug_mode ? "Right footing back" : "") << std::endl;
				r_foot_direction_front_back = false;
			}
			else if (r_foot_angle <= -25) {
				std::cout << (debug_mode ? "Right footing front" : "") << std::endl;
				r_foot_direction_front_back = true;
			}
			break;
		// HARD RESET
		case('R'):
			std::cout << "Hard Reset" << endl;
			body_translation = vec3(0, 0, 0);
			head_nod = 0;
			l_thigh_angle = 0;
			l_foot_angle = 0;
			r_thigh_angle = 0;
			r_foot_angle = 0;
			break;
	}

}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Teapot 4");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}











