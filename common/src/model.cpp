#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "model.hpp"
#include "utils.hpp"

void Model::clear() {
	this->vertices.clear();
	this->triangles.clear();
}

void Model::pushTriangle(int v1, int v2, int v3) {
	std::cout << "Added triangle " << v1 << " " << v2 << " " << v3 << "\n";
	this->triangles.push_back(v1);
	this->triangles.push_back(v2);
	this->triangles.push_back(v3);
}

void Model::generateTriangle(float px, float py, float pz) {
	this->clear();
	this->vertices.push_back(std::move(std::make_unique<Vertex>(px, 0.0f, 0.0f)));
	this->vertices.push_back(std::move(std::make_unique<Vertex>(0.0f, py, 0.0f)));
	this->vertices.push_back(std::move(std::make_unique<Vertex>(0.0f, 0.0f, pz)));
	this->triangles.push_back(0); this->triangles.push_back(1); this->triangles.push_back(2);
}

void Model::generatePlane(float dimension, int divisions) {
	this->clear();
	const int vertex_width = divisions + 1;
	const float start_dim = dimension * 0.5f;
	for(int row = 0; row < vertex_width; row++) {
		float z = -start_dim + row * (dimension / divisions);
		for(int column = 0; column < vertex_width; column++) {
			float x = -start_dim + column * (dimension / divisions);
			this->vertices.push_back(std::move(std::make_unique<Vertex>(x, 0.0f, z)));
			if (row != divisions && column != divisions) {
				const int current = row * vertex_width + column;
				this->pushTriangle(current, current + vertex_width, current + vertex_width + 1);
				this->pushTriangle(current, current + vertex_width + 1, current + 1);
			}
		}
	}
}
/* Comentários gerais:
	variável 'mapped_devices' permite indexação por matriz ser overlapping
		- vértice (0,0) da face traseira de uma box com 3 divisões, sendo o primeiro desta face, é na realidade o 17º do modelo
	Quantidade significativa de vértices duplicados
*/
void Model::generateBox(float dimension, int divisions) {
	this->clear();
	const int vertex_width = divisions + 1, vertices_per_face = vertex_width * vertex_width;
	const float start_dim = dimension * 0.5f;
	float x, y, z;
	int mapped_vertices = 0;
	/* Face frontal
		  3	+++++++++ 4
			+  	  + +				- Ortogonal ao eixo z
			+   +   +				- Vista de frente, por ordem de inicialização
			+ *     +
		  1	+++++++++ 2
	*/
	z = start_dim;
	for(int row = 0; row < vertex_width; row++) {
		y = -start_dim + row * (dimension / divisions);
		for(int column = 0; column < vertex_width; column++) {
			x = -start_dim + column * (dimension / divisions);
			this->vertices.push_back(std::make_unique<Vertex>(x, y, z));
			if (row != divisions && column != divisions) {
				int current = mapped_vertices + row * vertex_width + column;
				this->pushTriangle(current, current + 1, current + vertex_width + 1);
				this->pushTriangle(current, current + vertex_width + 1, current + vertex_width);
			}
		}
	}
	mapped_vertices += vertex_width * vertex_width;
	/* Face traseira
		  4	+++++++++ 3
			+	  +	+				- Ortogonal ao eixo z
			+	+	+				- Vista de frente, por ordem de inicialização
			+ +     +
		  2	+++++++++ 1
	*/
	z = -start_dim;
	for(int row = 0; row < vertex_width; row++) {
		y = -start_dim + row * (dimension / divisions);
		for(int column = 0; column < vertex_width; column++) {
			x = -start_dim + column * (dimension / divisions);
			this->vertices.push_back(std::make_unique<Vertex>(x, y, z));
			if (row != divisions && column != divisions) {
				int current = mapped_vertices + row * vertex_width + column;
				this->pushTriangle(current+1, current, current + vertex_width);
				this->pushTriangle(current+1, current + vertex_width, current + vertex_width + 1);
			}
		}
	}
	mapped_vertices += vertex_width * vertex_width;
	/* Face superior
		  1	+++++++++ 2
			+	  +	+				- Ortogonal ao eixo y
			+	+	+				- Vista de frente, por ordem de inicialização
			+ +     +
		  3	+++++++++ 4
	*/
	y = start_dim;
	for(int row = 0; row < vertex_width; row++) {
		z = -start_dim + row * (dimension / divisions);
		for(int column = 0; column < vertex_width; column++) {
			x = -start_dim + column * (dimension / divisions);
			this->vertices.push_back(std::make_unique<Vertex>(x, y, z));
			if (row != divisions && column != divisions) {
				int current = mapped_vertices + row * vertex_width + column;
				this->pushTriangle(current + vertex_width, current + 1, current);
				this->pushTriangle(current + vertex_width, current + vertex_width + 1, current + 1);
			}
		}
	}
	mapped_vertices += vertex_width * vertex_width;
	/* Face inferior
		  3	+++++++++ 4
			+	  +	+				- Ortogonal ao eixo y
			+	+	+				- Vista de frente, por ordem de inicialização
			+ +     +
		  1	+++++++++ 2
	*/
	y = -start_dim;
	for(int row = 0; row < vertex_width; row++) {
		z = -start_dim + row * (dimension / divisions);
		for(int column = 0; column < vertex_width; column++) {
			x = -start_dim + column * (dimension / divisions);
			this->vertices.push_back(std::make_unique<Vertex>(x, y, z));
			if (row != divisions && column != divisions) {
				int current = mapped_vertices + row * vertex_width + column;
				this->pushTriangle(current, current + 1, current + vertex_width + 1);
				this->pushTriangle(current, current + vertex_width + 1, current + vertex_width);
			}
		}
	}
	mapped_vertices += vertex_width * vertex_width;
	/* Face direita
		  4	+++++++++ 3
			+	  +	+				- Ortogonal ao eixo x
			+	+	+				- Vista de frente, por ordem de inicialização
			+ +     +
		  2	+++++++++ 1
	*/
	x = start_dim;
	for(int row = 0; row < vertex_width; row++) {
		y = -start_dim + row * (dimension / divisions);
		for(int column = 0; column < vertex_width; column++) {
			z = -start_dim + column * (dimension / divisions);
			this->vertices.push_back(std::make_unique<Vertex>(x, y, z));
			if (row != divisions && column != divisions) {
				int current = mapped_vertices + row * vertex_width + column;
				this->pushTriangle(current + 1, current, current + vertex_width);
				this->pushTriangle(current + 1, current + vertex_width, current + vertex_width + 1);
			}
		}
	}
	mapped_vertices += vertex_width * vertex_width;
	/* Face esquerda
		  3	+++++++++ 4
			+	  +	+			- Ortogonal ao eixo x
			+	+	+			- Vista de frente, por ordem de inicialização
			+ +     +
		  1	+++++++++ 2
	*/
	x = -start_dim;
	for(int row = 0; row < vertex_width; row++) {
		y = -start_dim + row * (dimension / divisions);
		for(int column = 0; column < vertex_width; column++) {
			z = -start_dim + column * (dimension / divisions);
			this->vertices.push_back(std::make_unique<Vertex>(x, y, z));
			if (row != divisions && column != divisions) {
				int current = mapped_vertices + row * vertex_width + column;
				this->pushTriangle(current, current + 1, current + vertex_width + 1);
				this->pushTriangle(current, current + vertex_width + 1, current + vertex_width);
			}
		}
	}
}

/* Comentários
	Iteração avalia as stacks dentro das slices, ou seja, a adição de vértices será da seguinte ordem
		stack y+1	  2					4			---> alpha incrementa para a diretia
		stack y		  1					3
					slice x			slice x+1
	É feito o unrolling dos loops tanto respectivos às stacks como das slices por duas razões:
		- a última slice "fecha" o cone
		- a última stack conecta-se não a outra stack mas ao pico do cone
	Cálculo das alturas intermédias (ys) é trivial; radii são obtidos pela lei de triângulos semelhantes
*/
void Model::generateCone(const float radius, const float height, const int slices, const int stacks) {
	this->clear();
	this->vertices.push_back(std::make_unique<Vertex>(0.0f, 0.0f, 0.0f));
	this->vertices.push_back(std::make_unique<Vertex>(0.0f, height, 0.0f));
	std::vector<float> ys (stacks), radii (stacks);
	for(int stack = 0; stack < stacks; stack++) {
		ys[stack] = (height / stacks) * stack;
		radii[stack] = radius * (1.0f - ys[stack] / height);
	}
	for(int slice = 0; slice < slices; slice++) {
		const float alpha = ((2 * M_PI) / slices) * slice;
		int current = 2 + slice * stacks;
		for(int stack = 0; stack < stacks; stack++) {
			current += stack;
			const float x = radii[stack] * sin(alpha), z = radii[stack] * cos(alpha);
			this->vertices.push_back(std::make_unique<Vertex>(x, ys[stack], z));
			int stack_neighbor = current + stacks;
			if (slice == slices - 1) {
				stack_neighbor = 2 + stack;
			}
			if (stack == stacks - 1) {
				this->pushTriangle(current, stack_neighbor, 1);
			} else {
				this->pushTriangle(current, stack_neighbor, current + 1);
				this->pushTriangle(current + 1, stack_neighbor, stack_neighbor + 1);
			}
			current -= stack;
		}
		if (slice == slices - 1) {
			this->pushTriangle(current, 0, 2);
		} else {
			this->pushTriangle(current, 0, current + stacks);
		}
	}
}
void Model::generateSphere(float radius, int slices, int stacks) {
	this->clear();
	this->vertices.push_back(std::make_unique<Vertex>(0.0f, radius, 0.0f));
	this->vertices.push_back(std::make_unique<Vertex>(0.0f, -radius, 0.0f));
	const int middle_vertices = stacks - 1; // por stack
	for(int slice = 0; slice < slices; slice++) {
		const float alpha = ((2 * M_PI) / slices) * slice;
		int current = 2 + middle_verticess * slice;
		if (slice == slices - 1) { this->pushTriangle(current, 2, 0); }
		else { this->pushTriangle(current, current + middle_vertices, 0); }
		for(int stack = 1; stack < stacks; stack++) {
			current++;
			const float beta = (M_PI / 2) - ((M_PI * slice)/slices);
			float z = radius * cos(beta) * cos(alpha), x = radius * cos(beta) * sin(alpha), radius * sin(beta);
		}
	}
}