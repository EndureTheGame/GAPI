#include "Renderer.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
x;\
ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[GL ERROR]" << error << "| " << function << "| " << file << "| " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& vao, const ElementBuffer& ebo, const Shader& shader) const
{
	shader.Bind();
	vao.Bind();
	ebo.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear()
{
	GlCall(glClearColor(1.0f, 0.5f, 0.2f, 1.0f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}
