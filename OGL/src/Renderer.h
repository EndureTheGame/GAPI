#pragma once

#include <glad.h>
#include <iostream>
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
x;\
ASSERT(GlLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
	void Draw(const VertexArray& vao, const ElementBuffer& ebo, const Shader& shader) const;
	void Clear();
};