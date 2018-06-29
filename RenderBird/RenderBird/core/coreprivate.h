#pragma once
#include <string>
#include <memory>
#include <assert.h>
#include <vector>
#include "math/mathcore.h"
using namespace MathLib;

#undef TYPE_ID
#define TYPE_ID(cls) TYPE_ID_##cls

#undef TYPE_ID_ITEM
#define	TYPE_ID_ITEM(cls,id)  TYPE_ID(cls) = id,

#undef TYPE_ID_EX
#define TYPE_ID_EX(cls) TYPE_ID_#cls

template<typename T>
class TSingleton
{
public:
	static T* GetInstance();
	static void Destroy();
private:
	TSingleton(TSingleton const&) {};
	TSingleton& operator=(TSingleton const&) {};

protected:
	static T* m_instance;
	TSingleton() { m_instance = static_cast <T*> (this); };
	~TSingleton() {  };
};

template<typename T>
typename T* TSingleton<T>::m_instance = 0;

template<typename T>
T* TSingleton<T>::GetInstance()
{
	if (!m_instance)
	{
		TSingleton<T>::m_instance = new T();
	}

	return m_instance;
}

template<typename T>
void TSingleton<T>::Destroy()
{
	delete TSingleton<T>::m_instance;
	TSingleton<T>::m_instance = 0;
}

class NonCopyable
{
public:
	NonCopyable() {}

private:
	NonCopyable(const NonCopyable&);
	NonCopyable& operator=(const NonCopyable&);
};