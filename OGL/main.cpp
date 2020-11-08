#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <String>
#include <fstream>
#include <sstream>


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

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
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


	window = glfwCreateWindow(WIDTH * 16 / 9, HEIGHT, title, NULL, NULL);
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


	float positions[6] = { -0.5f, -0.5f,
							0.5f, -0.5f,
							0.0f,  0.5f };

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	//Layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindVertexArray(0);

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
	std::cout << source.VertexShader << std::endl;
	std::cout << source.FragmentShader << std::endl;

	unsigned int shader = CreateShader(source.VertexShader, source.FragmentShader);


	int location = glGetUniformLocation(shader, "u_Color");
	_ASSERT(location != -1);
	glUniform4f(location, 0.5, 0, 1.0, 1.0);

	float r = 0.0f;
	float increment = 0.5f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
	
		/* Render here */
		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform4f(location, r, 0, 1.0, 1.0);

		if (r > 1.0f)
		{
			increment = -0.05f;
		}
		else if (r < 0.05f)
		{
			increment = 0.05f;
		}

		r += increment;

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwSwapInterval(1);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
