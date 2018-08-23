#include "renderbird_register.h"

void RegisterTypeInfoRenderBird()
{
	REGISTER_TYPEINFO(Material)
	REGISTER_TYPEINFO(Texture)
	REGISTER_TYPEINFO(TriangleMesh)
	REGISTER_TYPEINFO(LightProperty)
	REGISTER_TYPEINFO(DistantLightComponent)
	REGISTER_TYPEINFO(MeshComponent)
	REGISTER_TYPEINFO(CameraComponent)
	REGISTER_TYPEINFO(DiskComponent)
	REGISTER_TYPEINFO(AreaLight)
	REGISTER_TYPEINFO(SphereComponent)
}
