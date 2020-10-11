#include <iostream>
#include <String>


#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

int main()
{
	GLFWwindow* window;
	
	const char* title = "Test";
	static const int HEIGHT = 1920;
	static const int WIDTH = 1080;

	// initilizing library
	if (!glfwInit())
	{
		return -1;
	}
	glewInit();



	window = glfwCreateWindow(WIDTH * 16/9, HEIGHT, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(glewInit() != GLEW_OK)
	{
		std::cout << "ERROR" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;


	float positions[6] = { -0.5f, -0.5f,
						    0.0f, -0.5f,
						    0.5f, -0.5f };

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);

	//Layout
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwSwapInterval(1);

		/* Poll for and process events */
		glfwPollEvents();
	}


	glfwTerminate();

	return 0;
}