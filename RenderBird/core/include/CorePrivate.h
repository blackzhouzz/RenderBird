#pragma once
#include <string>
#include <memory>
#include <assert.h>
#include <vector>
#include <type_traits>
#include <malloc.h>
#include <memory>

#  ifdef CORE_DLL
#    ifdef CORE_DLL_INTERNAL
#      define CORE_PORT   __declspec(dllexport)
#    else
#      define CORE_PORT   __declspec(dllimport)
#    endif
#  endif

#ifndef CORE_PORT
#  define CORE_PORT
#endif

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

#define USE_HIGH_PRECISE
#ifdef USE_HIGH_PRECISE
using Float = double;
#else
using Float = float;
#endif

namespace Core
{
	template<typename T>
	class Singleton
	{
	public:
		static T* IntancePtr();
		static T& Instance();
		static void Destroy();
	private:
		Singleton(Singleton const&) {};
		Singleton& operator=(Singleton const&) {};

	protected:
		static T* m_instance;
		Singleton() { m_instance = static_cast <T*> (this); };
		~Singleton() {  };
	};

	template<typename T>
	typename T* Singleton<T>::m_instance = 0;

	template<typename T>
	T* Singleton<T>::IntancePtr()
	{
		if (!m_instance)
		{
			Singleton<T>::m_instance = new T();
		}

		return m_instance;
	}

	template<typename T>
	T& Singleton<T>::Instance()
	{
		if (!m_instance)
		{
			Singleton<T>::m_instance = new T();
		}

		return *m_instance;
	}

	template<typename T>
	void Singleton<T>::Destroy()
	{
		delete Singleton<T>::m_instance;
		Singleton<T>::m_instance = 0;
	}

	class NonCopyable
	{
	public:
		NonCopyable() {}

	private:
		NonCopyable(const NonCopyable&);
		NonCopyable& operator=(const NonCopyable&);
	};
}

template <typename T>
struct TypeId
{
	static const int value = 0;
};

#ifndef TYPE_ID_GROUP
#define TYPE_ID_GROUP(name, id) static const int TYPE_ID_GROUP_##name = id;
#endif

#ifndef TYPE_ID_CLASS
#define	TYPE_ID_CLASS(group, cls, id)\
	namespace group\
	{\
		class cls;\
	}\
	template <>\
	struct TypeId<group::cls>\
	{\
		static const int value = id + TYPE_ID_GROUP_##group;\
	};
#endif

#ifndef TYPE_ID_STRUCT
#define	TYPE_ID_STRUCT(group, cls, id)\
	namespace group\
	{\
		struct cls;\
	}\
	template <>\
	struct TypeId<group::cls>\
	{\
		static const int value = id + TYPE_ID_GROUP_##group;\
	};
#endif


#ifndef REGISTER_TYPEINFO
#define REGISTER_TYPEINFO(cls) \
	{ \
		extern void RegisterTypeInfo##cls(); \
		RegisterTypeInfo##cls(); \
	}


template<typename T>
inline std::unique_ptr<T[]> zeroAlloc(size_t size)
{
	std::unique_ptr<T[]> result(new T[size]);
	std::memset(result.get(), 0, size * sizeof(T));
	return std::move(result);
}

template<class T>
struct AlignedDeleter
{
	void operator()(T *p) const
	{
#ifdef _WIN32
		_aligned_free(p);
#else
		free(p);
#endif
	}
};

template<class T>
using aligned_unique_ptr = std::unique_ptr<T[], AlignedDeleter<T>>;

template<typename T>
inline aligned_unique_ptr<T> alignedAlloc(size_t size, int alignment)
{
	void *ptr;
#ifdef _WIN32
	ptr = _aligned_malloc(size * sizeof(T), alignment);
#else
	if (posix_memalign(&ptr, alignment, size * sizeof(T)))
		ptr = NULL;
#endif
	if (!ptr)
		return nullptr;
	return aligned_unique_ptr<T>(static_cast<T *>(ptr));
}

template<typename T>
inline aligned_unique_ptr<T> alignedZeroAlloc(size_t size, int alignment)
{
	auto result = alignedAlloc<T>(size, alignment);
	std::memset(result.get(), 0, size * sizeof(T));
	return std::move(result);
}

#endif
