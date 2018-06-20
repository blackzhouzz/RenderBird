#include "vector.h"
#include "coreprivate.h"
#include "typeinfo.h"
#include "object.h"
#include "component.h"
using namespace MathLib;
using namespace RenderBird;


class A
{
public:
	A()
	{
		printf("bb");
	}
	A(int i)
	{
		printf("aa");
	}
};
int main()
{
	printf(Component::ThisTypeInfo()->GetName().c_str());
	TypeInfo* info = Component::ThisTypeInfo();
	Object* pObj = info->Alloc();
	system("pause");
	return 0;
}