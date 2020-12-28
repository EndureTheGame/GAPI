#pragma once 

#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GlCall(glGenBuffers(1, &m_RendererID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

}
ElementBuffer::~ElementBuffer() 
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void ElementBuffer::Bind() const 
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}
void ElementBuffer::Unbind() const 
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}