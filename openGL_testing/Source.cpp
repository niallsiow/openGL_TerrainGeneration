#include <GL\glew.h>
#include <GLFW\glfw3.h>

// standard library headers
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

#include "shader_s.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// glm headers
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



void GenerateLineOfPoints(float* pointLine, float spaceBetweenPoints, int numberOfPoints);

void GenerateGridOfPoints(float* pointGrid, int gridHeight, int gridLength, float spaceBetweenPoints);

void CenterGrid(float* pointGrid, int indicesInPointGrid, int gridHeight, int gridLength, float spaceBetweenPoints);

void GenerateGridIndices(unsigned int* indiceArray, int gridLength, int indices);

void Create_Y_Sine_Displacement(float* pointGrid, int indices, float displacement, float smoothness);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main(void)
{
	GLFWwindow* window;



	if (!glfwInit())
		return -1;




	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	if (glewInit() != GLEW_OK) {
		std::cout << "error" << std::endl;
	}

	// glViewPort will resize openGL render to args 3 and 4, and will place the bottom left corner with args 1 and 2
	// glViewport(200, 150, 400, 300);

	glViewport(0, 0, 1920, 1080);


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Shader ourShader("vertex.vs", "fragmentShader.fs");

	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);


	if (data) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	float ebo_vertices[] = {
		// positions			// colors			// texture co-ordinates
		 0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f	// top left

	};


	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	float spaceBetweenPoints = 0.1f;
	int gridHeight = 20;
	int gridLength = 40;
	int indicesInPointGrid = gridHeight * gridLength * 3;
	float* pointGrid = new float[indicesInPointGrid];

	GenerateGridOfPoints(pointGrid, gridHeight, gridLength, spaceBetweenPoints);
	CenterGrid(pointGrid, indicesInPointGrid, gridHeight, gridLength, spaceBetweenPoints);
	Create_Y_Sine_Displacement(pointGrid, indicesInPointGrid, 0.2f, 5.0f);


	int numberOfTrianglesInGrid = 2 * (gridHeight - 1) * (gridLength - 1);
	int indicesToDrawGridTriangles = numberOfTrianglesInGrid * 3;

	unsigned int*  gridIndices = new unsigned int[indicesToDrawGridTriangles];

	GenerateGridIndices(gridIndices, gridLength, indicesToDrawGridTriangles);


	
	

	// make a vertex buffer object to store grid vertices
	unsigned int gridVAO;
	glGenVertexArrays(1, &gridVAO);
	unsigned int gridVBO;
	glGenBuffers(1, &gridVBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(gridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * indicesInPointGrid, pointGrid, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesToDrawGridTriangles, gridIndices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	


	// camera stuff
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp;

	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		// input
		processInput(window);

		// Rendering Commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		int modelMatrixLocation = glGetUniformLocation(ourShader.ID, "modelMatrix");
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -5.0f));
		int viewMatrixLocation = glGetUniformLocation(ourShader.ID, "viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		int projectionMatrixLocation = glGetUniformLocation(ourShader.ID, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		ourShader.use();


		// draw grid
		glBindVertexArray(gridVAO);

		glDrawElements(GL_TRIANGLES, indicesToDrawGridTriangles, GL_UNSIGNED_INT, 0);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	
	// deallocate memory
	delete[] pointGrid;
	delete[] gridIndices;

	glfwTerminate();
	return 0;
}

void GenerateLineOfPoints(float* pointLine, float spaceBetweenPoints, int numberOfPoints) {
	int indices = numberOfPoints * 3;
	for (int i = 0; i < indices; i++) {
		if (i % 3 == 0) {
			pointLine[i] = (spaceBetweenPoints / 3) * i;
		}
		else {
			pointLine[i] = 0.0f;
		}
	}
}

void GenerateGridOfPoints(float* pointGrid, int gridHeight, int gridLength, float spaceBetweenPoints) {

	int pointsInLine = gridLength;
	int indicesInPointLineArray = pointsInLine * 3;

	float* pointLine = new float[indicesInPointLineArray];

	GenerateLineOfPoints(pointLine, spaceBetweenPoints, pointsInLine);

	int increment = 0;

	for (int j = 0; j < gridHeight; j++) {
		for (int i = 0; i < indicesInPointLineArray; i++) {
			if (i % 3 == 2) {
				pointGrid[increment] = j * spaceBetweenPoints;
			}
			else {
				pointGrid[increment] = pointLine[i];
			}
			increment++;
		}
	}

	delete[] pointLine;
}

void CenterGrid(float* pointGrid, int indicesInPointGrid, int gridHeight, int gridLength, float spaceBetweenPoints) {
		// center point grid
	for (int i = 0; i < indicesInPointGrid; i++) {
		// displace x axis
		if (i % 3 == 0) {
			pointGrid[i] = pointGrid[i] - ((spaceBetweenPoints * gridLength) / 2);
		}
		// displace z axis
		else if (i % 3 == 2) {
			pointGrid[i] = pointGrid[i] - ((spaceBetweenPoints * (gridHeight - 1)) / 2);
		}
	}
}

void GenerateGridIndices(unsigned int* indiceArray, int gridLength, int indices) {
	unsigned int quadIndices[]{
	0, gridLength, 1,
	1, gridLength, (gridLength + 1)
	};

	int increment = 0;
	for (int j = 0; j < (indices / 6); j++) {
		for (int i = 0; i < 6; i++) {
			indiceArray[increment] = quadIndices[i] + (j * 1);
			increment++;
		}
	}
}

void Create_Y_Sine_Displacement(float* pointGrid, int indices, float displacement, float smoothness) {
	smoothness = 1 / smoothness;
	for (int i = 0; i < indices; i++) {
		if (i % 3 == 1) {
			pointGrid[i] = sin(i * smoothness) * displacement;
		}
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

