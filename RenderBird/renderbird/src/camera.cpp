#include "camera.h"
#include "math/mathutils.h"

namespace RenderBird
{
	CameraBase::CameraBase()
		: m_needUpdateProj(true)
		, m_needUpdateView(true)
		, m_view(Matrix4f::ZERO)
		, m_proj(Matrix4f::ZERO)
		, m_viewOrientation(Quaternion::IDENTITY)
	{
	}

	CameraBase::CameraBase(const Vector3f& viewPos, Float nearClip, Float farClip)
		: m_needUpdateProj(true)
		, m_needUpdateView(true)
		, m_view(Matrix4f::ZERO)
		, m_proj(Matrix4f::ZERO)
		, m_viewOrientation(Quaternion::IDENTITY)
	{
		SetViewPosition(viewPos);
		SetNearClip(nearClip);
		SetFarClip(farClip);
	}

	CameraBase::~CameraBase()
	{
	}

	void CameraBase::MoveForward(Float units)
	{
		if (units != 0)
		{
			MoveRelative(Vector3f(0, 0, units));
		}
	}

	void CameraBase::MoveRight(Float units)
	{
		if (units != 0)
		{
			MoveRelative(Vector3f(units, 0, 0));
		}
	}

	void CameraBase::MoveUp(Float units)
	{
		if (units != 0)
		{
			MoveRelative(Vector3f(0, units, 0));
		}
	}

	void CameraBase::MoveRelative(const Vector3f& vec)
	{
		Vector3f vTrans = MathUtils::RotateVector(m_viewOrientation, vec);
		m_viewPos += vTrans;
		m_needUpdateView = true;
	}

	Float CameraBase::GetPitch()const
	{
		return m_viewOrientation.GetPitch();
	}

	Float CameraBase::GetYaw()const
	{
		return m_viewOrientation.GetYaw();
	}

	Float CameraBase::GetRoll()const
	{
		return m_viewOrientation.GetRoll();
	}

	void CameraBase::Rotate(const Vector3f& axis, Float angle)
	{
		if (angle == 0)
		{
			return;
		}
		Quaternion kQuat;
		kQuat = MathUtils::AngleAxisToQuaternion(angle, axis);
		Rotate(kQuat);
	}

	void CameraBase::Rotate(const Quaternion& quat)
	{
		m_viewOrientation = quat * m_viewOrientation;
		m_needUpdateView = true;
	}

	void CameraBase::LookAt(const Vector3f& eyePos, const Vector3f& at, const Vector3f& up)
	{
		Vector3f axisZ = (at - eyePos).Normalize();
		Vector3f axisX = Cross(up, axisZ).Normalize();
		if (axisX.LengthSQ() < C_FLOAT_EPSILON)
		{
			axisX = Vector3f::UNIT_X;
		}
		Vector3f axisY = Cross(axisZ, axisX).Normalize();

		SetViewPosition(eyePos);
		m_viewOrientation = MathUtils::AxesToQuaternion(axisX, axisY, axisZ);
		m_needUpdateView = true;
	}

	void CameraBase::LookAt(const Vector3f& eyePos, Float pitch, Float yaw, Float roll)
	{
		Matrix3f matRotation = MathUtils::EulerAnglesYXZToRotationMatrix(pitch, yaw, roll);
		m_viewOrientation = MathUtils::RotationMatrixToQuaternion(matRotation);
		SetViewPosition(eyePos);
		m_needUpdateView = true;
	}

	const Vector3f& CameraBase::GetViewPosition()const
	{
		return m_viewPos;
	}

	const Quaternion& CameraBase::GetViewRotation()const
	{
		return m_viewOrientation;
	}

	void CameraBase::SetViewRotation(const Quaternion& rotate)
	{
		m_viewOrientation = rotate;
		m_needUpdateView = true;
	}

	void CameraBase::SetViewPosition(const Vector3f& position)
	{
		m_viewPos = position;
		m_needUpdateView = true;
	}

	Vector3f CameraBase::GetViewDirection()const
	{
		Vector4f vec = GetViewMatrix().GetColumn((uint32)Axis::Z);
		return Vector3f(vec.x, vec.y, vec.z);
	}

	Float CameraBase::GetNearClip()const
	{
		return m_nearClip;
	}

	void CameraBase::SetNearClip(Float nearClip)
	{
		m_nearClip = nearClip;
		m_needUpdateProj = true;
	}

	Float CameraBase::GetFarClip()const
	{
		return m_farClip;
	}

	void CameraBase::SetFarClip(Float farClip)
	{
		m_farClip = farClip;
		m_needUpdateProj = true;
	}

	void CameraBase::UpdateMatrix(bool forceUpdate)const
	{
		UpdateViewMatrix(forceUpdate);
		UpdateProjMatrix(forceUpdate);
	}

	void CameraBase::UpdateViewMatrix(bool forceUpdate)const
	{
		if (m_needUpdateView || forceUpdate)
		{
			Matrix3f rot;
			rot = MathUtils::QuaternionToRotationMatrix(m_viewOrientation);

			m_view = Matrix4f::IDENTITY;
			m_view = rot;

			Vector3f trans = m_viewPos * rot;
			m_view.SetTranslation(-trans);
			m_needUpdateView = false;
		}
	}

	void CameraBase::UpdateProjMatrix(bool forceUpdate)const
	{
		if (m_needUpdateProj || forceUpdate)
		{
			UpdateProjMatrixImpl();
			m_needUpdateProj = false;
		}
	}

	const Matrix4f& CameraBase::GetViewMatrix()const
	{
		UpdateViewMatrix();
		return m_view;
	}

	const Matrix4f& CameraBase::GetProjMatrix()const
	{
		UpdateProjMatrix();
		return m_proj;
	}

	PerspectiveCamera::PerspectiveCamera()
		: CameraBase()
		, mAspect(0)
		, mFovY(0)
	{
	}

	PerspectiveCamera::PerspectiveCamera(
		const Vector3f& viewPos,
		Float nearClip,
		Float farClip,
		Float fovY,
		Float aspect
	)
	: CameraBase(viewPos, nearClip, farClip)
	{
		SetFovY(fovY);
		SetAspect(aspect);
	}

	void PerspectiveCamera::UpdateProjMatrixImpl()const
	{
		m_proj = MathUtils::PerspectiveFovMatrix(
			DegToRad(mFovY),
			mAspect,
			m_nearClip,
			m_farClip
		);
	}

	void PerspectiveCamera::PerspectiveFovLH(
		Float fovY,
		Float aspect,
		Float nearClip,
		Float farClip)
	{
		SetFovY(fovY);
		SetAspect(aspect);
		SetNearClip(nearClip);
		SetFarClip(farClip);
	}

	void PerspectiveCamera::SetAspect(Float aspect)
	{
		mAspect = aspect;
		m_needUpdateProj = true;
	}

	Float PerspectiveCamera::GetAspect()const
	{
		return mAspect;
	}

	void PerspectiveCamera::SetFovY(Float fovY)
	{
		mFovY = fovY;
		m_needUpdateProj = true;
	}

	Float PerspectiveCamera::GetFovY()const
	{
		return mFovY;
	}

	Float PerspectiveCamera::GetNearWidth()const
	{
		return GetNearHeight() * mAspect;
	}

	Float PerspectiveCamera::GetNearHeight()const
	{
		return 2.0 * std::tan(DegToRad(mFovY) * 0.5) * m_nearClip;
	}

	Float PerspectiveCamera::GetFarWidth()const
	{
		return GetFarHeight() * mAspect;
	}

	Float PerspectiveCamera::GetFarHeight()const
	{
		return m_farClip / m_nearClip * GetNearHeight();
	}


	OrthoCamera::OrthoCamera()
		: m_orthoRect(Vector4f::ZERO)
	{
	}

	OrthoCamera::OrthoCamera(const Vector3f& viewPos, Float nearClip, Float farClip, Float width, Float height)
		: CameraBase(viewPos, nearClip, farClip)
	{
		OrthoLH(width, height, nearClip, farClip);
	}

	void OrthoCamera::UpdateProjMatrixImpl()const
	{
		m_proj = MathUtils::OrthoOffCenterMatrix(
			m_orthoRect.x,
			m_orthoRect.z,
			m_orthoRect.w,
			m_orthoRect.y,
			m_nearClip,
			m_farClip
		);
	}

	void OrthoCamera::OrthoOffCenterLH(
		Float left,
		Float right,
		Float bottom,
		Float top,
		Float nearClip,
		Float farClip
	)
	{
		m_orthoRect.x = left;
		m_orthoRect.z = right;
		m_orthoRect.w = bottom;
		m_orthoRect.y = top;
		m_nearClip = nearClip;
		m_farClip = farClip;
		m_needUpdateProj = true;
	}

	void OrthoCamera::OrthoLH(Float width, Float height, Float nearClip, Float farClip)
	{
		Float halfWidth = width * 0.5;
		Float halfHeight = height * 0.5;
		m_orthoRect.x = -halfWidth;
		m_orthoRect.z = halfWidth;
		m_orthoRect.w = -halfHeight;
		m_orthoRect.y = halfHeight;
		m_nearClip = nearClip;
		m_farClip = farClip;
		m_needUpdateProj = true;
	}

	Float OrthoCamera::GetLeft()const
	{
		return m_orthoRect.x;
	}

	void OrthoCamera::SetLeft(Float left)
	{
		m_orthoRect.x = left;
		m_needUpdateProj = true;
	}

	Float OrthoCamera::GetTop()const
	{
		return m_orthoRect.y;
	}

	void OrthoCamera::SetTop(Float nearClip)
	{
		m_orthoRect.y = nearClip;
		m_needUpdateProj = true;
	}

	Float OrthoCamera::GetRight()const
	{
		return m_orthoRect.z;
	}

	void OrthoCamera::SetRight(Float right)
	{
		m_orthoRect.z = right;
		m_needUpdateProj = true;
	}

	Float OrthoCamera::GetBottom()const
	{
		return m_orthoRect.w;
	}

	void OrthoCamera::SetBottom(Float bottom)
	{
		m_orthoRect.w = bottom;
		m_needUpdateProj = true;
	}

	Float OrthoCamera::GetWidth()const
	{
		return m_orthoRect.z - m_orthoRect.x;
	}

	Float OrthoCamera::GetHeight()const
	{
		return m_orthoRect.w - m_orthoRect.y;
	}
}