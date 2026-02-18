#include <glad/glad.h>
#include "BufferObjects.h"


namespace leo
{
	static GLenum BufferUsageToOpenGLFlag(BufferUsage usage)
	{
		switch (usage)
		{
		case leo::BufferUsage::Static:
			return GL_STATIC_DRAW;
			break;
		case leo::BufferUsage::Dynamic:
			return GL_DYNAMIC_DRAW;
			break;
		case leo::BufferUsage::Stream:
			return GL_STREAM_DRAW;
			break;
		}

		return GL_STATIC_DRAW;
	}

	// ---------------- VertexBuffer ----------------

	VertexBuffer::VertexBuffer(const void* data, u32 size, BufferUsage usage)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageToOpenGLFlag(usage));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		:
		m_id(other.m_id)
	{
		other.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		glDeleteBuffers(1, &m_id);

		m_id = other.m_id;
		other.m_id = 0;
		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// ---------------- IndexBuffer ----------------

	IndexBuffer::IndexBuffer(const u32* data, u32 count, BufferUsage usage)
		:
		m_count(count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), (const void*)data, BufferUsageToOpenGLFlag(usage));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
		:
		m_count(other.m_count),
		m_id(other.m_id)
	{
		other.m_id = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		glDeleteBuffers(1, &m_id);

		m_id = other.m_id;
		m_count = other.m_count;

		other.m_id = 0;

		return *this;
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// ---------------- VertexArray ----------------

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept
		:
		m_id(other.m_id)
	{
		other.m_id = 0;
		m_buffers = std::move(other.m_buffers);
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		glDeleteVertexArrays(1, &m_id);

		m_id = other.m_id;
		other.m_id = 0;

		m_buffers = std::move(other.m_buffers);

		return *this;
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddAttrib(u32 i, ElementType element_type, u32 stride, u32& offset, bool per_instance)
	{
		VertexAttributeDesc desc = GetAttributeDesc(element_type);

		if (element_type == ElementType::MAT4)
		{
			for (u32 j = 0; j < 4; j++)
			{
				glEnableVertexAttribArray(i + j);
				glVertexAttribPointer(i + j, desc.count, desc.type, desc.normalized,
					stride, reinterpret_cast<void*>(offset + j * sizeof(glm::vec4)));
				glVertexAttribDivisor(i + j, 1);
			}
			offset += sizeof(glm::mat4);
			return;
		}

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, desc.count, desc.type, desc.normalized, stride, reinterpret_cast<void*>((u64)offset));
		glVertexAttribDivisor(i, per_instance ? 1 : 0);
		offset += desc.size_bytes;
	}

}
