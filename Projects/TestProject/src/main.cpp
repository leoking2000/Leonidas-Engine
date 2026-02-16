#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <LEO/LeoEngine.h>

static void init(GLFWwindow* window) 
{
	LEOLOGINFO("Hello!");
}

static void display(GLFWwindow* window, double currentTime) 
{
	glClearColor(0.2, 0.2, 0.9, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) 
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Leonidas Engine", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}