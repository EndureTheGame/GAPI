#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <String>
#include <fstream>
#include <sstream>

#include "stb_image.h"

//#########################################################################################################

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
x;\
ASSERT(GlLogCall(#x, __FILE__, __LINE__))

static void GlClearError ()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[GL ERROR]" << error << "| " << function << "| " << file << "| " << line << std::endl;
		return false;
	}
	return true;
}

//#######################################################################################################

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#Shader") != std::string::npos)
		{
			if (line.find("Vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("Fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n'; 
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//Error Handle
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to Compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);

		return 0;
	}


	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));
	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return program;
}

void loadImage()
{
	//load image
	GLsizei width, height, channels;
	stbi_set_flip_vertically_on_load(true);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


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


	

	float positions[] = {	//Positions		//Color						//Texture
							0.5f,  0.5f,	1.0f, 0.0f, 0.0f, 1.0f,  	1.0f, 1.0f,	 // top right
							0.5f, -0.5f,	0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 0.0f,	 // bottom right
						   -0.5f, -0.5f,	0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f,	 // bottom left
						   -0.5f,  0.5f,	0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f}; // top left
	
	unsigned int indices[] = {
							0, 1, 3,
							1, 2, 3 };


	//int location = glGetUniformLocation(shader, "u_Color");
	//glUniform4f(location, 0.5f, 0.0f, 1.0f, 1.0f);

	//BUFFER ID
	unsigned int VBO, VAO, EBO;

	//Gen Buffer/s
	GlCall(glGenBuffers(1, &EBO));
	GlCall(glGenBuffers(1, &VBO));
	GlCall(glGenVertexArrays(1, &VAO));
	
	//Vertex Array Bind
	GlCall(glBindVertexArray(VAO));

	//Vertex Buffer Bind
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	//Element(Vertex) Array Bind
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	//Layout

	//Possitions
	GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0));
	GlCall(glEnableVertexAttribArray(0));
		  
	//Colo(r
	GlCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(2 * sizeof(float))));
	GlCall(glEnableVertexAttribArray(1));
		  
	//Text(ure
	GlCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float))));
	GlCall(glEnableVertexAttribArray(2));
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		/*
	std::string vertexShader =
		"#version 330 core\n"
		"layout(location = 0)in vec4 position;  \n"
		"void main()                \n"
		"{                          \n"
		"  gl_Position = position; \n"
		"};                         \n";

	std::string fragmentShader =
		"#version 330 core\n"
		"layout(location = 0)out vec4 color;  \n"
		"void main()                \n"
		"{                          \n"
		"  color = vec4(0.0, 0.0, 1.0, 1.0); \n"
		"};							\n";


		*/


	ShaderProgramSource source = ParseShader("BasicShader.shader");
	unsigned int shader = CreateShader(source.VertexShader, source.FragmentShader);

	loadImage();
	unsigned int texture;
	GlCall(glGenTextures(1, &texture));
	GlCall(glBindTexture(GL_TEXTURE_2D, texture));
	loadImage();
	GlCall(glUseProgram(shader));
	GlCall(glUniform1i(glGetUniformLocation(shader, "u_Texture"), 0));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
	
		/* Render here */
		GlCall(glClearColor(1.0f, 0.5f, 0.2f, 1.0f));
		GlCall(glClear(GL_COLOR_BUFFER_BIT));
		
		
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//glUniform4f(location, 0.0f, greenValue, 0.0f, 1.0f);
		GlCall(glActiveTexture(GL_TEXTURE0));
		GlCall(glBindTexture(GL_TEXTURE_2D, texture)); 
			  
		GlCall(glUseProgram(shader));
		GlCall(glBindVertexArray(VAO));
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwSwapInterval(1);

		/* Poll for and process events */
		glfwPollEvents();
	}
	GlCall(glBindVertexArray(0));
	GlCall(glDeleteVertexArrays(1, &VAO));
	GlCall(glDeleteBuffers(1, &VBO));
	GlCall(glDeleteBuffers(1, &EBO));
	GlCall(glDeleteProgram(shader));
	glfwTerminate();
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}