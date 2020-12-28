#pragma once
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Undbind() const;

};