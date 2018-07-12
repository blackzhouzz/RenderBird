#pragma once
#include <string>
#include <memory>
#include <assert.h>
#include <vector>
#include "math/mathcore.h"

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