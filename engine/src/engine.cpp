#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include "utils.hpp"
#include "state.h"

static State state;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(state.fov, ratio, state.near, state.far);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawAxes() {
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f, 0.0f, 0.0f);
	glVertex3f( 100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

void drawModel(const Model *model) {
	glBegin(GL_TRIANGLES);
	glColor3f(0.5f, 0.5f, 0.5f);
	std::cout << model->triangles.size() << " and " << model->vertices.size() << "\n";
	for(int i = 0; i < model->triangles.size(); i++) {
		const int vertex_i = model->triangles[i];
		// std::cout << "At " << i << ", vertex " << vertex_i <<"\n";
		Vertex *vertex = model->vertices[vertex_i].get();
		glVertex3f(vertex->x, vertex->y, vertex->z);
	}
	glEnd();
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(state.position_x,state.position_y, state.position_z,
		      state.lookAt_x, state.lookAt_y, state.lookAt_z,
			  state.up_x, state.up_y, state.up_z);

	Model m;
	m.generatePlane(1.0f, 3);
	// m.generateBox(2.0f, 3);
	// m.generateCone(1.0f, 2.0f, 4, 3);
	// m.generateSphere(1.0f, 10, 10);
	drawModel(&m);
	drawAxes();
	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
}


int main(int argc, char** argv)
{
	state.loadConfig("config.xml");

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(state.width,state.height);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT,GL_LINE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}