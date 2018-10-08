#pragma once
#include "RenderbirdPrivate.h"

namespace RenderBird
{
	template<typename T>
	class Pixmap
	{
	public:
		Pixmap() = default;
		Pixmap(uint32 width, uint32 height, T* buffer = nullptr)
			: m_width(width)
			, m_height(height) 
			, m_buffer(malloc(sizeof(T) * width * height))
		{
			if (buffer != nullptr)
			{
				memcpy(m_buffer->get(), buffer, sizeof(T) * width * height);
			}
		}
		void Reset()
		{
			m_width = m_height = 0;
			m_buffer.reset();
		}

		void Clear()
		{
			std::memset(m_buffer.get(), nullptr, GetSizeInBytes());
		}

		T* GetBuffer()const { return m_buffer->get(); }
		const T& operator[](int index)const { return m_buffer[index]; }
		inline T& operator[](int index) { return m_buffer[index]; }
		inline T &operator()(int x, int y) { return m_buffer[x + y * m_width]; }
		inline T  operator()(int x, int y) const { return m_buffer[x + y * m_width]; }

		uint32 GetSizeInBytes()const { return m_width * m_height * sizeof(T); }
		uint32 GetSize()const { return m_width * m_height; }
		uint32 GetWidth()const { return m_width; }
		uint32 GetHeight()const { return m_height; }
	private:
		std::unique_ptr<T> m_buffer;
		uint32 m_width;
		uint32 m_height;
	};

	typedef Pixmap<Vector3f> Pixmap3f;
	typedef Pixmap<float> PixmapF;
}