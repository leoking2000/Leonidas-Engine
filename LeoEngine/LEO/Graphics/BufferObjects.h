#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <LEO/Utilities/LeoTypes.h>
#include <LEO/Log/Log.h>

namespace leo
{
	enum class BufferUsage
	{
		Static,
		Dynamic,
		Stream
	};

	enum class ElementType
	{
		FLOAT1, FLOAT1_N,
		FLOAT2, FLOAT2_N,
		FLOAT3, FLOAT3_N,
		FLOAT4, FLOAT4_N,
		UCHAR3, UCHAR3_N,
		UCHAR4, UCHAR4_N,
		MAT4
	};

	struct VertexAttributeDesc
	{
		GLenum type;
		u32 count;
		bool normalized;
		u32 size_bytes;
	};

	inline VertexAttributeDesc GetAttributeDesc(ElementType t)
	{
		switch (t)
		{
		case ElementType::FLOAT1:  return { GL_FLOAT, 1, false, sizeof(float) };
		case ElementType::FLOAT1_N:return { GL_FLOAT, 1, true, sizeof(float) };
		case ElementType::FLOAT2:  return { GL_FLOAT, 2, false, 2 * sizeof(float) };
		case ElementType::FLOAT2_N:return { GL_FLOAT, 2, true, 2 * sizeof(float) };
		case ElementType::FLOAT3:  return { GL_FLOAT, 3, false, 3 * sizeof(float) };
		case ElementType::FLOAT3_N:return { GL_FLOAT, 3, true, 3 * sizeof(float) };
		case ElementType::FLOAT4:  return { GL_FLOAT, 4, false, 4 * sizeof(float) };
		case ElementType::FLOAT4_N:return { GL_FLOAT, 4, true, 4 * sizeof(float) };
		case ElementType::UCHAR3:  return { GL_UNSIGNED_BYTE, 3, false, 3 * sizeof(unsigned char) };
		case ElementType::UCHAR3_N:return { GL_UNSIGNED_BYTE, 3, true, 3 * sizeof(unsigned char) };
		case ElementType::UCHAR4:  return { GL_UNSIGNED_BYTE, 4, false, 4 * sizeof(unsigned char) };
		case ElementType::UCHAR4_N:return { GL_UNSIGNED_BYTE, 4, true, 4 * sizeof(unsigned char) };
		case ElementType::MAT4:    return { GL_FLOAT, 4, false, sizeof(glm::vec4) * 4 };
		default: LEOASSERT(false, "Unknown ElementType"); return { 0,0,false,0 };
		}
	}

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, u32 size, BufferUsage usage = BufferUsage::Static);

		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		~VertexBuffer();
	public:
		void Bind() const;
		void UnBind() const;
	private:
		u32 m_id;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const u32* data, u32 count, BufferUsage usage = BufferUsage::Static);

		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;

		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;

		~IndexBuffer();
	public:
		void Bind() const;
		void UnBind() const;

		inline u32 GetCount() const { return m_count; }
	private:
		u32 m_id;
		u32 m_count;
	};

	/*
	* Represents the attribute layout of a VBO
	*/
	template<u32 ELEMENTS_COUNT>
	class Layout
	{
	public:
		Layout(ElementType* arr)
		{
			LEOASSERT(arr != nullptr, "Element type array is null");
			m_stride = 0;
			for (u32 i = 0; i < ELEMENTS_COUNT; i++)
			{
				m_arr[i] = arr[i];
				m_stride += GetAttributeDesc(arr[i]).size_bytes;
			}
		}

		inline ElementType operator[](int i) const { return m_arr[i]; }
		inline u32 GetStride() const { return m_stride; }
		inline u32 GetCount() const { return ELEMENTS_COUNT; }
	private:
		ElementType m_arr[ELEMENTS_COUNT];
		u32 m_stride;
	};

	class VertexArray
	{
	public:
		VertexArray();

		VertexArray(const VertexArray& other) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;

		~VertexArray();
	public:
		void Bind() const;
		void UnBind() const;
		inline u32 ID() const { return m_id; }
	public:
		template<u32 ELEMENTS_COUNT>
		void AddBuffer(VertexBuffer&& vb, const Layout<ELEMENTS_COUNT>& layout, u32 start = 0, bool per_instance = false)
		{
			Bind();
			vb.Bind();
			u32 offset = 0;
			for (u32 i = start; i < start + ELEMENTS_COUNT; i++)
			{
				AddAttrib(i, layout[i - start], layout.GetStride(), offset, per_instance);
			}
			UnBind();
			vb.UnBind();
			m_buffers.emplace_back(std::move(vb));
		}

		void SetIndexBuffer(IndexBuffer&& ib)
		{
			Bind();
			ib.Bind();
			m_indexBuffer = std::move(ib);
			UnBind();
		}
	private:
		void AddAttrib(u32 i, ElementType element_type, u32 stride, u32& offset, bool per_instance);

		u32 m_id;
		std::vector<VertexBuffer> m_buffers;
		IndexBuffer m_indexBuffer = IndexBuffer(nullptr, 0); // default empty
	};
}