#include <cstdlib>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <cmath>
#include "utils.hpp"
#include "state.h"


static State state;

struct Mundo {
	std::vector<Model> models;
};

static Mundo mundo;

// Variáveis para controle da câmera em coordenadas esféricas
static float camera_alpha = 0.0f;  // Ângulo horizontal (rotação ao redor do eixo Y)
static float camera_beta = 0.0f;   // Ângulo vertical (elevação)
static float camera_zoom = 5.0f; 

void updateCameraPosition() {
	// Converter coordenadas esféricas para cartesianas
	state.position_x = state.lookAt_x + camera_zoom * cos(camera_beta) * sin(camera_alpha);
	state.position_y = state.lookAt_y + camera_zoom * sin(camera_beta);
	state.position_z = state.lookAt_z + camera_zoom * cos(camera_beta) * cos(camera_alpha);
	
	glutPostRedisplay();
}

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
	for(size_t i = 0; i < model->triangles.size(); i++) {
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

	// Desenhar todos os modelos do mundo
	for (const auto& model : mundo.models) {
		drawModel(&model);
	}
	
	drawAxes();
	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
	const float angle_step = 0.1f;  // Passo de rotação em radianos (~5.7 graus)
	const float radius_step = 0.5f; // Passo para zoom
	
	switch(c) {
		case 'a':  // Rodar para a esquerda
		case 'A':
			camera_alpha -= angle_step;
			updateCameraPosition();
			break;
		case 'd':  // Rodar para a direita
		case 'D':
			camera_alpha += angle_step;
			updateCameraPosition();
			break;
		case 'w':  // Rodar para cima
		case 'W':
			camera_beta += angle_step;
			if(camera_beta > 1.5f) camera_beta = 1.5f;  // Limitar a ~85 graus
			updateCameraPosition();
			break;
		case 's':  // Rodar para baixo
		case 'S':
			camera_beta -= angle_step;
			if(camera_beta < -1.5f) camera_beta = -1.5f;  // Limitar a ~-85 graus
			updateCameraPosition();
			break;
		case '+':  // Aproximar (zoom in)
			camera_zoom -= radius_step;
			if(camera_zoom < 1.0f) camera_zoom = 1.0f;  // Distância mínima
			updateCameraPosition();
			break;
		case '-':  // Afastar (zoom out)
			camera_zoom += radius_step;
			updateCameraPosition();
			break;
	}
}


int main(int argc, char** argv)
{
	state.loadConfig("config.xml");
	
	// Copiar os modelos do state para o mundo
	mundo.models = std::move(state.models);
	
	// Calcular a posição inicial da câmera em coordenadas esféricas
	float dx = state.position_x - state.lookAt_x;
	float dy = state.position_y - state.lookAt_y;
	float dz = state.position_z - state.lookAt_z;
	
	camera_zoom = sqrt(dx*dx + dy*dy + dz*dz);
	camera_beta = asin(dy / camera_zoom);
	camera_alpha = atan2(dx, dz);

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