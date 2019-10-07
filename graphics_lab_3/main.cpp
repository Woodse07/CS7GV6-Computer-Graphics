
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "maths_funcs.h" //Anton's math class
#include "teapot.h" // teapot mesh
#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>


//typedef double DWORD;


// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;
GLuint shaderProgramID;

unsigned int teapot_vao = 0;
int width = 800.0;
int height = 600.0;
GLuint vertex_position_location;
GLuint vertex_normals_location;

// Model Matrices
mat4 model_bl = identity_mat4();
mat4 model_tl = rotate_x_deg(identity_mat4(), 90);

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS

std::string readShaderSource(const std::string& fileName)
{
	std::ifstream file(fileName.c_str()); 
	if(file.fail()) {
		cout << "error loading shader called " << fileName;
		exit (1); 
	} 
	
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	return stream.str();
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }
	std::string outShader = readShaderSource(pShaderText);
	const char* pShaderSource = outShader.c_str();

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

void generateObjectBufferTeapot () {
	GLuint vp_vbo = 0;

	vertex_position_location = glGetAttribLocation(shaderProgramID, "vertex_position");
	vertex_normals_location = glGetAttribLocation(shaderProgramID, "vertex_normals");
	
	glGenBuffers (1, &vp_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof (float), teapot_vertex_points, GL_STATIC_DRAW);
	GLuint vn_vbo = 0;
	glGenBuffers (1, &vn_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof (float), teapot_normals, GL_STATIC_DRAW);
  
	glGenVertexArrays (1, &teapot_vao);
	glBindVertexArray (teapot_vao);

	glEnableVertexAttribArray (vertex_position_location);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer (vertex_position_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (vertex_normals_location);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer (vertex_normals_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}


#pragma endregion VBO_FUNCTIONS


void display(){

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram (shaderProgramID);


	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation (shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation (shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation (shaderProgramID, "proj");
	int ortho_mat_location = glGetUniformLocation(shaderProgramID, "ortho");
	

	//Here is where the code for the viewport lab will go, to get you started I have drawn a t-pot in the bottom left
	//The model transform rotates the object by 45 degrees, the view transform sets the camera at -40 on the z-axis, and the perspective projection is setup using Antons method

	// bottom-left
	mat4 view_bl = translate (identity_mat4 (), vec3 (0.0, 0.0, -40.0));
	mat4 persp_proj_bl = perspective(45.0, (float)width/(float)height, 0.1, 100.0);
	mat4 ortho_bl = identity_mat4();
	model_bl = rotate_y_deg (model_bl, 0.05);

	glViewport (0, 0, width/2, height/2);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj_bl.m);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_bl.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model_bl.m);
	glUniformMatrix4fv(ortho_mat_location, 1, GL_FALSE, ortho_bl.m);
	glDrawArrays (GL_TRIANGLES, 0, teapot_vertex_count);

	// bottom-right
	mat4 view_br = translate(identity_mat4(), vec3(0.0, 0.0, -40.0));
	mat4 persp_proj_br = perspective(75, (float)width / (float)height, 11.0, 80.0);
	mat4 ortho_br = identity_mat4();
	mat4 model_br = rotate_x_deg(identity_mat4(), 20.0);
	model_br = rotate_z_deg(model_br, -20.0);
	model_br = rotate_y_deg(model_br, -45.0);

	glViewport(width/2, 0, width/2, height/2);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj_br.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_br.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model_br.m);
	glUniformMatrix4fv(ortho_mat_location, 1, GL_FALSE, ortho_br.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	// top-left
	mat4 view_tl = translate(identity_mat4(), vec3(0.0, 0.0, -50.0));
	mat4 persp_proj_tl = perspective(45.0, (float)width / (float)height, 0.1, 100.0);
	mat4 ortho_tl = ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	model_tl = rotate_z_deg(model_tl, 0.05);

	glViewport(0, height/2, width/2, height/2);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj_tl.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_tl.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model_tl.m);
	glUniformMatrix4fv(ortho_mat_location, 1, GL_FALSE, ortho_tl.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);

	// top-right
	mat4 view_tr = translate(identity_mat4(), vec3(0.0, 0.0, -40.0));
	mat4 persp_proj_tr = perspective(45.0, (float)width / (float)height, 0.1, 100.0);
	mat4 ortho_tr = identity_mat4();
	mat4 model_tr = rotate_z_deg(identity_mat4(), 0);

	glViewport(width/2, height/2, width/2, height/2);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj_tr.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_tr.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model_tr.m);
	glUniformMatrix4fv(ortho_mat_location, 1, GL_FALSE, ortho_tr.m);
	glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
    glutSwapBuffers();
}


void updateScene() {	

		// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;
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

int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Viewport Teapots");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);

	 // A call to glewInit() must be done after glut is initialized!
	glewExperimental = GL_TRUE; //for non-lab machines, this line gives better modern GL support
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











