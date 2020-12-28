
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad.h>

#include <iostream>
#include <String>
#include <fstream>
#include <sstream>

#include "stb_image.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void loadImage()
{
	//load image
	GLsizei width, height, channels;
	stbi_set_flip_vertically_on_load(true);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	unsigned char* data = stbi_load("res/container.jpg", &width, &height, &channels, 4);
	if (data)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GlCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed To Load The Image File!" << std::endl;
	}
}

int main(void)
{
	GLFWwindow* window;

	const char* title = "Test";
	int HEIGHT = 1920;
	int WIDTH = 1080;

	// initilizing library
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {	//Positions		//Color						//Texture
								0.5f,  0.5f,	1.0f, 0.0f, 0.0f, 1.0f,		1.0f * 4, 1.0f * 4,	  // top right
								0.5f, -0.5f,	0.0f, 1.0f, 0.0f, 1.0f,		1.0f * 4, 0.0f * 4,	  // bottom right
							   -0.5f, -0.5f,	0.0f, 0.0f, 1.0f, 1.0f,		0.0f * 4, 0.0f * 4,	  // bottom left
							   -0.5f,  0.5f,	1.0f, 1.0f, 0.0f, 1.0f,		0.0f * 4, 1.0f * 4 }; // top left

		unsigned int indices[] = {
								0, 1, 3,
								1, 2, 3 };


		VertexArray va;

		VertexBuffer vb(positions, 4 * 2 * sizeof(positions));
		vb.Bind();

		ElementBuffer ebo(indices, sizeof(indices));
		ebo.Bind();
		
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		layout.Push<float>(2);
		va.addBuffer(vb, layout);
		
		/*Layout
		//Possitions
		//GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
		//GlCall(glEnableVertexAttribArray(0));

		//Color
		//GlCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(2 * sizeof(float))));
		//GlCall(glEnableVertexAttribArray(1));

		//Texture
		//GlCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)))); 
		//GlCall(glEnableVertexAttribArray(2));
		*/
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
		Shader shader("BasicShader.shader");
		shader.Bind();

		unsigned int texture;
		GlCall(glGenTextures(1, &texture));
		GlCall(glBindTexture(GL_TEXTURE_2D, texture));
		loadImage();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GlCall(glClearColor(1.0f, 0.5f, 0.2f, 1.0f));
			GlCall(glClear(GL_COLOR_BUFFER_BIT));

			float timeValue = glfwGetTime();
			float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
			float redValue = (sin(timeValue) / 1.0f * cos(timeValue/2)) + 0.5f;
			float BlueValue = (cos(timeValue) / 1.65f) + 0.5f;
			shader.Setuniform4f("u_Color", redValue, greenValue, BlueValue, 1.0f);

			GlCall(glActiveTexture(GL_TEXTURE0));
			GlCall(glBindTexture(GL_TEXTURE_2D, texture));

			shader.Bind();
			GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			va.Bind();
			//glDrawArrays(GL_TRIANGLES, 0, 3);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
			glfwSwapInterval(1);

			/* Poll for and process events */
			glfwPollEvents();
		}
		va.Undbind();
		vb.Unbind();
		ebo.Unbind();
		shader.Unbind();
	}
	glfwTerminate();
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}