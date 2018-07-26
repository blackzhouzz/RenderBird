#include "runtime_register.h"
#include "transform.h"
#include "typeinfo.h"
//#include "object.h"

void RegisterTypeInfoRuntime()
{
	REGISTER_TYPEINFO(Object)
	REGISTER_TYPEINFO(Transform)
}
