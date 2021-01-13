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

// camera
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool isWireframeActive = false;

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


	Shader ourShader("Shaders/smoothSine.vert", "Shaders/fragmentShader.frag");




	float spaceBetweenPoints = 0.05f;
	int gridHeight = 80;
	int gridLength = 80;
	int indicesInPointGrid = gridHeight * gridLength * 3;
	float* pointGrid = new float[indicesInPointGrid];

	GenerateGridOfPoints(pointGrid, gridHeight, gridLength, spaceBetweenPoints);
	CenterGrid(pointGrid, indicesInPointGrid, gridHeight, gridLength, spaceBetweenPoints);
	// Create_Y_Sine_Displacement(pointGrid, indicesInPointGrid, 0.1f, 5.0f);

	

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
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	glEnable(GL_DEPTH_TEST);

	const float radius = 10.0f;

	float time;


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		// input
		processInput(window);

		// Rendering Commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// grid stuff
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		int modelMatrixLocation = glGetUniformLocation(ourShader.ID, "modelMatrix");
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		int viewMatrixLocation = glGetUniformLocation(ourShader.ID, "viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		int projectionMatrixLocation = glGetUniformLocation(ourShader.ID, "projectionMatrix");
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		
		time = glfwGetTime();
		ourShader.setFloat("time", time);


		// draw grid
		glBindVertexArray(gridVAO);
		glDrawElements(GL_TRIANGLES, indicesToDrawGridTriangles, GL_UNSIGNED_INT, 0);
		

		ourShader.use();

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

	int offset = 0;
	int increment = 0;
	for (int j = 0; j < indices / 6; j++) {
		if (j != 0 && j % (gridLength - 1) == 0) {
			offset = offset + 1;
		}
		for (int i = 0; i < 6; i++) {
			indiceArray[increment] = quadIndices[i] + j + offset;
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
	// close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// camera input
	float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}

	// toggle wireframe mode
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (isWireframeActive == true) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isWireframeActive = false;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			isWireframeActive = true;
		}
	}

}

