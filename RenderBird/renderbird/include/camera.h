#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	class CameraBase
	{
	public:
		CameraBase();
		CameraBase(const Vector3f& viewPos, Float nearClip, Float farClip);
		virtual ~CameraBase();
		void MoveForward(Float units);
		void MoveRight(Float units);
		void MoveUp(Float units);
		void MoveRelative(const Vector3f& vec);
		Float GetPitch()const;
		Float GetYaw()const;
		Float GetRoll()const;
		void Rotate(const Vector3f& axis, Float angle);				//逆时针绕axis轴旋转angle
		void Rotate(const Quaternion& quat);						//逆时针旋转四元数形式
		void LookAt(const Vector3f& eyePos, const Vector3f& at, const Vector3f& up);
		void LookAt(const Vector3f& eyePos, Float pitch, Float yaw, Float roll);
		const Vector3f& GetViewPosition()const;
		const Quaternion& GetViewRotation()const;
		void SetViewRotation(const Quaternion& rotate);
		void SetViewPosition(const Vector3f& position);
		Vector3f GetViewDirection()const;
		Float GetNearClip()const;
		void SetNearClip(Float nearClip);
		Float GetFarClip()const;
		void SetFarClip(Float farClip);
		void UpdateMatrix(bool forceUpdate = false)const;
		void UpdateViewMatrix(bool forceUpdate = false)const;
		void UpdateProjMatrix(bool forceUpdate = false)const;
		const Matrix4f& GetViewMatrix()const;
		const Matrix4f& GetProjMatrix()const;
	protected:
		virtual void UpdateProjMatrixImpl()const = 0;
	protected:
		mutable bool m_needUpdateProj;
		mutable bool m_needUpdateView;
		mutable Matrix4f m_view;
		mutable Matrix4f m_proj;
		Quaternion m_viewOrientation;
		Vector3f m_viewPos;
		Float m_nearClip;
		Float m_farClip;
	};

	class PerspectiveCamera : public CameraBase
	{
	public:
		PerspectiveCamera();
		PerspectiveCamera(
			const Vector3f& viewPos,
			Float nearClip,
			Float farClip,
			Float fovY,
			Float aspect
		);
		void PerspectiveFovLH(
			Float fovY,
			Float aspect,
			Float nearClip,
			Float farClip
		);
		void SetAspect(Float aspect);
		Float GetAspect()const;
		void SetFovY(Float fovY);
		Float GetFovY()const;
		Float GetNearWidth()const;
		Float GetNearHeight()const;
		Float GetFarWidth()const;
		Float GetFarHeight()const;
	protected:
		virtual void UpdateProjMatrixImpl()const;
	protected:
		Float mAspect;			//width/height
		Float mFovY;			//垂直fov角度（弧度制）
	};

	class OrthoCamera : public CameraBase
	{
	public:
		OrthoCamera();
		OrthoCamera(const Vector3f& viewPos, Float nearClip, Float farClip, Float width, Float height);
		void OrthoOffCenterLH(
			Float left,
			Float right,
			Float bottom,
			Float top,
			Float nearClip,
			Float farClip);
		void OrthoLH(Float width, Float height, Float nearClip, Float farClip);
		Float GetLeft()const;
		void SetLeft(Float left);
		Float GetTop()const;
		void SetTop(Float nearClip);
		Float GetRight()const;
		void SetRight(Float right);
		Float GetBottom()const;
		void SetBottom(Float bottom);
		Float GetWidth()const;
		Float GetHeight()const;
	protected:
		virtual void UpdateProjMatrixImpl()const;
	protected:
		Vector4f m_orthoRect;		//x==left y==right z==top w==bottom
	};
}

