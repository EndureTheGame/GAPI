#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() 
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
	GlCall(glBindVertexArray(m_RendererID));
}
VertexArray::~VertexArray()
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& element = layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < element.size(); i++)
	{
		const auto& elements = element[i];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, elements.count, elements.type, elements.normalized, layout.GetStride(), (const void*)offset));
		offset += elements.count * ElementBufferLayout::GetSizeOfType(elements.type);
	}
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}
void VertexArray::Undbind() const
{
	GlCall(glBindVertexArray(0));
}