#pragma once
#include <string>
#include <memory>
#include <assert.h>
#include <vector>

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
#endif
