#include "math/mathutils.h"

namespace Core
{
	Matrix4f MathUtils::TranslateMatrix(const Vector3f& trans)
	{
		return Matrix4f(
			1.0, 0, 0, 0,
			0, 1.0, 0, 0,
			0, 0, 1.0, 0,
			trans.x, trans.y, trans.z, 1.0);
	}

	Matrix4f MathUtils::ScaleMatrix(const Vector3f& scale)
	{
		return Matrix4f(
			scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			0, 0, 0, 1.0);
	}

	Matrix4f MathUtils::ScaleMatrix(Float scale)
	{
		return Matrix4f(
			scale, 0, 0, 0,
			0, scale, 0, 0,
			0, 0, scale, 0,
			0, 0, 0, 1.0);
	}

	Matrix4f MathUtils::InverseRotationMatrix(Float pitch, Float yaw, Float roll)
	{
		return Matrix4f(
			Matrix4f(
				+std::cos(yaw), -std::sin(yaw), 0, 0,
				+std::sin(yaw), +std::cos(yaw), 0, 0,
				0, 0, 1.0, 0,
				0, 0, 0, 1.0
			) *
			Matrix4f(
				+std::cos(pitch), 0, -std::sin(pitch), 0,
				0, 1.0, 0, 0,
				+std::sin(pitch), 0, +std::cos(pitch), 0,
				0, 0, 0, 1.0
			) *
			Matrix4f(
				1.0, 0, 0, 0,
				0, +std::cos(roll), +std::sin(roll), 0,
				0, -std::sin(roll), +std::cos(roll), 0,
				0, 0, 0, 1.0)
		);
	}

	Matrix4f MathUtils::RotationMatrix(const Vector3f& axis, Float theta)
	{
		Float cosTheta = std::cos(theta);
		Float oneMinusCosTheta = 1.f - cosTheta;
		Float sinTheta = std::sin(theta);
		return Matrix4f(
			axis.x * axis.x * oneMinusCosTheta + cosTheta,
			axis.x * axis.y * oneMinusCosTheta + axis.z * sinTheta,
			axis.x * axis.z * oneMinusCosTheta - axis.y * sinTheta,
			0,
			axis.x * axis.y * oneMinusCosTheta - axis.z * sinTheta,
			axis.y * axis.y * oneMinusCosTheta + cosTheta,
			axis.y * axis.z * oneMinusCosTheta + axis.x * sinTheta,
			0,
			axis.x * axis.z * oneMinusCosTheta + axis.y * sinTheta,
			axis.y * axis.z * oneMinusCosTheta - axis.x * sinTheta,
			axis.z * axis.z * oneMinusCosTheta + cosTheta,
			0,
			0,
			0,
			0,
			1.0
		);
	}

	Matrix4f MathUtils::ScaleDirectionMatrix(const Vector3f& dir, Float k)
	{
		return Matrix4f(
			1.0 + (k - 1.0) * dir.x * dir.x, (k - 1.0) * dir.x * dir.y, (k - 1.0) * dir.x * dir.z, 0,
			(k - 1.0) * dir.x * dir.y, 1.0 + (k - 1.0) * dir.y * dir.y, (k - 1.0) * dir.y * dir.z, 0,
			(k - 1.0) * dir.x * dir.z, (k - 1.0) * dir.z * dir.y, 1.0 + (k - 1.0) * dir.z * dir.z, 0,
			0, 0, 0, 1.0);
	}

	Matrix4f MathUtils::BasisMatrix(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ)
	{
		return Matrix4f(
			axisX.x, axisY.x, axisZ.x, 0,
			axisX.y, axisY.y, axisZ.y, 0,
			axisX.z, axisY.z, axisZ.z, 0,
			0, 0, 0, 1.0
		);
	}

	Matrix4f MathUtils::OrthoMatrixLH(Float width, Float height, Float nearClip, Float farClip)
	{
		return Matrix4f(
			2.0 / width, 0, 0, 0,
			0, 2.0 / height, 0, 0,
			0, 0, 1.0 / (farClip - nearClip), 0,
			0, 0, nearClip / (nearClip - farClip), 1.0
		);
	}

	Matrix4f MathUtils::OrthoOffCenterMatrixLH(Float left, Float right, Float bottom, Float top, Float nearClip, Float farClip)
	{
		return Matrix4f(
			2.0 / (right - left), 0, 0, 0,
			0, 2.0 / (top - bottom), 0, 0,
			0, 0, 1.0 / (farClip - nearClip), 0,
			(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0
		);
	}

	Matrix4f MathUtils::PerspectiveFovMatrixLH(Float fovY, Float aspect, Float nearClip, Float farClip)
	{
		Float scaleY = 1.0 / std::tan(fovY * 0.5);
		Float scaleX = scaleY / aspect;
		return Matrix4f(
			scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, farClip / (farClip - nearClip), 1.0,
			0, 0, -nearClip * farClip / (farClip - nearClip), 0
		);
	}

	Matrix4f MathUtils::PerspectiveFovInfiniteMatrixLH(Float fovY, Float aspect, Float nearClip)
	{
		Float scaleY = 1.0 / std::tan(fovY * 0.5);
		Float scaleX = scaleY / aspect;
		return Matrix4f(
			scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, (1.0 - C_FLOAT_EPSILON), 1.0,
			0, 0, -nearClip * (1.0 - C_FLOAT_EPSILON), 0
		);
	}

	Matrix4f MathUtils::LookAtMatrixLH(const Vector3f& eyePos, const Vector3f& at, const Vector3f& up)
	{
		Vector3f axisZ = (at - eyePos).Normalize();
		Vector3f axisX = Cross(up, axisZ).Normalize();
		Vector3f axisY = Cross(axisZ, axisX);
		return Matrix4f(
			axisX.x, axisY.x, axisZ.x, 0.0,
			axisX.y, axisY.y, axisZ.y, 0.0,
			axisX.z, axisY.z, axisZ.z, 0.0,
			-Dot(axisX, eyePos), -Dot(axisY, eyePos), -Dot(axisZ, eyePos), 1.0
		);
	}

	Matrix4f MathUtils::LookAtMatrix(const Vector3f& eyePos, const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ)
	{
		return Matrix4f(
			axisX.x, axisY.x, axisZ.x, 0.0,
			axisX.y, axisY.y, axisZ.y, 0.0,
			axisX.z, axisY.z, axisZ.z, 0.0,
			-Dot(axisX, eyePos), -Dot(axisY, eyePos), -Dot(axisZ, eyePos), 1.0
		);
	}

	Quaternion MathUtils::Slerp(Float t, const Quaternion& p, const Quaternion& quat, bool shortestPath)
	{
		Float cosTemp = p.Dot(quat);
		Quaternion rt;

		// Do we need to invert rotation?
		if (cosTemp < 0.0 && shortestPath)
		{
			cosTemp = -cosTemp;
			rt = -quat;
		}
		else
		{
			rt = quat;
		}

		if (abs(cosTemp) < 1 - C_FLOAT_EPSILON)
		{
			// Standard case (slerp)
			Float sinTemp = std::sqrt(1.0 - cosTemp * cosTemp);
			Float angle = std::atan2(sinTemp, cosTemp);
			Float invSin = 1.0 / sinTemp;
			Float coeff0 = std::sin((1.0 - t) * angle) * invSin;
			Float coeff1 = std::sin(t * angle) * invSin;
			return coeff0 * p + coeff1 * rt;
		}
		else
		{
			// There are two situations:
			// 1. "p" and "quat" are very close (cosTemp ~= +1), so we can do a linear
			//    interpolation safely.
			// 2. "p" and "quat" are almost inverse of each other (cosTemp ~= -1), there
			//    are an infinite number of possibilities interpolation. but we haven't
			//    have method to fix this case, so just use linear interpolation here.
			Quaternion q = (1.0 - t) * p + t * rt;
			// taking the complement requires renormalisation
			q.Normalize();
			return q;
		}
	}

	Quaternion MathUtils::ExtraRotation(const Matrix4f& mat)
	{
		return RotationMatrixToQuaternion(mat.GetMatrix3());
	}

	Quaternion MathUtils::RotationMatrixToQuaternion(const Matrix3f& mat)
	{
		//<<3d数学>>实现方式

		//Quaternion ret;
		//Float fWM1 = mat[0][0] + mat[1][1] + mat[2][2];
		//Float fXM1 = mat[0][0] - mat[1][1] - mat[2][2];
		//Float fYM1 = mat[1][1] - mat[0][0] - mat[2][2];
		//Float fZM1 = mat[2][2] - mat[0][0] - mat[1][1];

		//int iBiggestIndex = 0;
		//Float fBiggestM1 = fWM1;
		//if (fXM1 > fBiggestM1)
		//{
		//	fBiggestM1 = fXM1;
		//	iBiggestIndex = 1;
		//}
		//if (fYM1 > fBiggestM1)
		//{
		//	fBiggestM1 = fYM1;
		//	iBiggestIndex = 2;
		//}
		//if (fZM1 > fBiggestM1)
		//{
		//	fBiggestM1 = fZM1;
		//	iBiggestIndex = 3;
		//}
		//Float fBiggestVal = sqrt(fBiggestM1+1.0)*0.5;
		//Float fMult = 0.25f / fBiggestVal;
		//switch(iBiggestIndex)
		//{
		//case 0:
		//	ret.w = fBiggestVal;
		//	ret.x = (mat[1][2] - mat[2][1]) * fMult;
		//	ret.y = (mat[2][0] - mat[0][2]) * fMult;
		//	ret.z = (mat[0][1] - mat[1][0]) * fMult;
		//	break;
		//case 1:
		//	ret.x = fBiggestVal;
		//	ret.w = (mat[1][2] - mat[2][1]) * fMult;
		//	ret.y = (mat[0][1] + mat[1][0]) * fMult;
		//	ret.z = (mat[2][0] + mat[0][2]) * fMult;
		//	break;
		//case 2:
		//	ret.y = fBiggestVal;
		//	ret.w = (mat[2][0] - mat[0][2]) * fMult;
		//	ret.x = (mat[0][1] + mat[1][0]) * fMult;
		//	ret.z = (mat[1][2] + mat[2][1]) * fMult;
		//	break;
		//case 3:
		//	ret.z = fBiggestVal;
		//	ret.w = (mat[0][1] - mat[1][0]) * fMult;
		//	ret.x = (mat[2][0] + mat[0][2]) * fMult;
		//	ret.y = (mat[1][2] + mat[2][1]) * fMult;
		//	break;
		//}
		//return ret;

		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".
		Quaternion ret;
		Float trace = mat[0][0] + mat[1][1] + mat[2][2];
		Float root;

		if (trace > 0.0)
		{
			// |w| > 1/2, may as well choose w > 1/2
			root = (Float)sqrt(trace + 1.0);  // 2w
			ret.w = 0.5 * root;
			root = 0.5 / root;  // 1/(4w)
			ret.x = (mat[2][1] - mat[1][2]) * root;
			ret.y = (mat[0][2] - mat[2][0]) * root;
			ret.z = (mat[1][0] - mat[0][1]) * root;
		}
		else
		{
			// |w| <= 1/2
			static unsigned int sNext[3] = { 1, 2, 0 };
			unsigned int i = 0;
			if (mat[1][1] > mat[0][0])
				i = 1;
			if (mat[2][2] > mat[i][i])
				i = 2;
			unsigned int j = sNext[i];
			unsigned int k = sNext[j];

			root = (Float)std::sqrt(mat[i][i] - mat[j][j] - mat[k][k] + 1.0);
			Float* apkQuat[3] = { &ret.x, &ret.y, &ret.z };
			*apkQuat[i] = 0.5 * root;
			root = 0.5 / root;
			ret.w = (mat[k][j] - mat[j][k])*root;
			*apkQuat[j] = (mat[j][i] + mat[i][j])*root;
			*apkQuat[k] = (mat[k][i] + mat[i][k])*root;
		}
		return ret;
	}

	Matrix3f MathUtils::EulerAnglesYXZToRotationMatrix(Float pitch, Float yaw, Float roll)
	{
		Float cosTemp, sinTemp;

		cosTemp = std::cos(yaw);
		sinTemp = std::sin(yaw);
		Matrix3f matY(cosTemp, 0.0, sinTemp, 0.0, 1.0, 0.0, -sinTemp, 0.0, cosTemp);

		cosTemp = std::cos(pitch);
		sinTemp = std::sin(pitch);
		Matrix3f matX(1.0, 0.0, 0.0, 0.0, cosTemp, -sinTemp, 0.0, sinTemp, cosTemp);

		cosTemp = std::cos(roll);
		sinTemp = std::sin(roll);
		Matrix3f matZ(cosTemp, -sinTemp, 0.0, sinTemp, cosTemp, 0.0, 0.0, 0.0, 1.0);

		return matY * (matX * matZ);
	}

	bool MathUtils::RotationMatrixToEulerAnglesYXZ(const Matrix3f& rotate, Float& pitch, Float& yaw, Float& roll)
	{
		// rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
		//        cx*sz           cx*cz          -sx
		//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

		pitch = std::asin(-rotate.m[1][2]);
		if (pitch < C_HALF_PI)
		{
			if (pitch > -C_HALF_PI)
			{
				yaw = std::atan2(rotate.m[0][2], rotate.m[2][2]);
				roll = std::atan2(rotate.m[1][0], rotate.m[1][1]);
				return true;
			}
			else
			{
				// WARNING.  Not a unique solution.
				Float rmY = std::atan2(-rotate.m[0][1], rotate.m[0][0]);
				roll = (0.0);  // any angle works
				yaw = roll - rmY;
				return false;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			Float rpY = std::atan2(-rotate.m[0][1], rotate.m[0][0]);
			roll = (0.0);  // any angle works
			yaw = rpY - roll;
			return false;
		}
	}

	bool MathUtils::ComputeBarycentricCoords(const Vector3f vert[], const Vector3f& position, Vector3f& b)
	{
		const Vector3f d1 = vert[1] - vert[0];
		const Vector3f d2 = vert[2] - vert[1];
		Vector3f norm = AbsVector(Cross(d1, d2));
		int projU, projV;
		if ((norm.x > norm.y) && (norm.x > norm.z))
		{
			projU = 1;
			projV = 2;
		}
		else if (norm.y > norm.z)
		{
			projU = 2;
			projV = 0;
		}
		else
		{
			projU = 0;
			projV = 1;
		}
		Float u1 = vert[0][projU] - vert[2][projU];
		Float u2 = vert[1][projU] - vert[2][projU];
		Float u3 = position[projU] - vert[0][projU];
		Float u4 = position[projU] - vert[2][projU];

		Float v1 = vert[0][projV] - vert[2][projV];
		Float v2 = vert[1][projV] - vert[2][projV];
		Float v3 = position[projV] - vert[0][projV];
		Float v4 = position[projV] - vert[2][projV];

		Float denom = v1 * u2 - v2 * u1;
		if (std::abs(denom) < C_FLOAT_EPSILON)
			return false;

		Float invDenom = 1.0 / denom;
		b.x = (v4 * u2 - v2 * u4) * invDenom;
		b.y = (v1 * u3 - v3 * u1) * invDenom;
		b.z = 1.0 - (b.x + b.y);
		return true;
	}

	Matrix3f MathUtils::QuaternionToRotationMatrix(const Quaternion& quat)
	{
		Matrix3f matRot;
		Float tx = 2.0 * quat.x;
		Float ty = 2.0 * quat.y;
		Float tz = 2.0 * quat.z;
		Float twx = tx * quat.w;
		Float twy = ty * quat.w;
		Float twz = tz * quat.w;
		Float txx = tx * quat.x;
		Float txy = ty * quat.x;
		Float txz = tz * quat.x;
		Float tyy = ty * quat.y;
		Float tyz = tz * quat.y;
		Float tzz = tz * quat.z;

		matRot[0][0] = 1.0 - (tyy + tzz);
		matRot[0][1] = txy - twz;
		matRot[0][2] = txz + twy;
		matRot[1][0] = txy + twz;
		matRot[1][1] = 1.0 - (txx + tzz);
		matRot[1][2] = tyz - twx;
		matRot[2][0] = txz - twy;
		matRot[2][1] = tyz + twx;
		matRot[2][2] = 1.0 - (txx + tyy);

		//matRot[0][0] = 1.0-(tyy+tzz);
		//matRot[1][0] = txy-twz;
		//matRot[2][0] = txz+twy;
		//matRot[0][1] = txy+twz;
		//matRot[1][1] = 1.0-(txx+tzz);
		//matRot[2][1] = tyz-twx;
		//matRot[0][2] = txz-twy;
		//matRot[1][2] = tyz+twx;
		//matRot[2][2] = 1.0-(txx+tyy);
		return matRot;
	}

	Quaternion MathUtils::AngleAxisToQuaternion(Float angle, const Vector3f& axis)
	{
		// assert:  axis[] is unit length
		//
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		Quaternion ret;
		Float halfAngle(0.5 * angle);
		Float sinTemp = std::sin(halfAngle);

		ret.x = sinTemp * axis.x;
		ret.y = sinTemp * axis.y;
		ret.z = sinTemp * axis.z;
		ret.w = std::cos(halfAngle);
		return ret;
	}

	void MathUtils::QuaternionToAngleAxis(const Quaternion& quat, Float& angle, Vector3f& axis)
	{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		Float sqLength = quat.x*quat.x + quat.y*quat.y + quat.z*quat.z;
		if (sqLength > 0.0)
		{
			angle = 2.0*std::acos(quat.w);
			Float invLength = 1.0 / (std::sqrt(sqLength));
			axis.x = quat.x * invLength;
			axis.y = quat.y * invLength;
			axis.z = quat.z * invLength;
		}
		else
		{
			// angle is 0 (mod 2*pi), so any axis will do
			angle = 0.0;
			axis.x = 1.0;
			axis.y = 0.0;
			axis.z = 0.0;
		}
	}

	Quaternion MathUtils::AxesToQuaternion(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ)
	{
		Matrix3f rot;

		rot[0][0] = axisX.x;
		rot[1][0] = axisX.y;
		rot[2][0] = axisX.z;

		rot[0][1] = axisY.x;
		rot[1][1] = axisY.y;
		rot[2][1] = axisY.z;

		rot[0][2] = axisZ.x;
		rot[1][2] = axisZ.y;
		rot[2][2] = axisZ.z;

		return RotationMatrixToQuaternion(rot);
	}

	void MathUtils::QuaternionToAxes(const Quaternion& quat, Vector3f& axisX, Vector3f& axisY, Vector3f& axisZ)
	{
		Matrix3f rot;

		rot = QuaternionToRotationMatrix(quat);

		axisX.x = rot[0][0];
		axisX.y = rot[1][0];
		axisX.z = rot[2][0];

		axisY.x = rot[0][1];
		axisY.y = rot[1][1];
		axisY.z = rot[2][1];

		axisZ.x = rot[0][2];
		axisZ.y = rot[1][2];
		axisZ.z = rot[2][2];
	}

	EulerRotation MathUtils::QuaternionToEulerRotation(const Quaternion& rotate)
	{
		EulerRotation ret;
		ret.pitch = asin(Clamp(2 * (rotate.w * rotate.x - rotate.y * rotate.z), -1.0, 1.0));
		ret.yaw = atan2(2 * (rotate.w * rotate.y + rotate.z * rotate.x), 1 - 2 * (rotate.x * rotate.x + rotate.y * rotate.y));
		ret.roll = atan2(2 * (rotate.w * rotate.z + rotate.x * rotate.y), 1 - 2 * (rotate.z * rotate.z + rotate.x * rotate.x));
		return ret;
	}

	Quaternion MathUtils::EulerRotationToQuaternion(const EulerRotation& rotate)
	{
		Quaternion ret;
		Float  angle;
		Float  sinRoll, sinPitch, sinYaw, cosRoll, cosPitch, cosYaw;

		angle = rotate.yaw * 0.5;
		sinYaw = std::sin(angle);
		cosYaw = std::cos(angle);

		angle = rotate.pitch * 0.5;
		sinPitch = std::sin(angle);
		cosPitch = std::cos(angle);

		angle = rotate.roll * 0.5;
		sinRoll = std::sin(angle);
		cosRoll = std::cos(angle);

		ret.x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		ret.y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		ret.z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		ret.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

		ret.Normalize();
		return ret;
	}

	Matrix4f MathUtils::MakeTransform(const Vector3f& position, const Quaternion& orient, const Vector3f& scale)
	{
		Matrix4f ret;
		Matrix3f rot3x3, scale3x3;
		rot3x3 = QuaternionToRotationMatrix(orient);
		scale3x3 = Matrix3f::ZERO;
		scale3x3[0][0] = scale.x;
		scale3x3[1][1] = scale.y;
		scale3x3[2][2] = scale.z;

		ret = scale3x3 * rot3x3;
		ret.SetTranslation(position);

		ret.m[0][3] = 0;
		ret.m[1][3] = 0;
		ret.m[2][3] = 0;
		ret.m[3][3] = 1;
		return ret;
	}

	Matrix4f MathUtils::MakeTransform(const Vector3f& position, const EulerRotation& pitchYawRoll, const Vector3f& scale)
	{
		Matrix4f ret;
		Matrix3f rot3x3, scale3x3;
		const Vector3f radPitchYawRoll = ToRadian(pitchYawRoll);
		rot3x3 = EulerAnglesYXZToRotationMatrix(radPitchYawRoll.x, radPitchYawRoll.y, radPitchYawRoll.z);
		scale3x3 = Matrix3f::ZERO;
		scale3x3[0][0] = scale.x;
		scale3x3[1][1] = scale.y;
		scale3x3[2][2] = scale.z;

		ret = scale3x3 * rot3x3;
		ret.SetTranslation(position);

		ret.m[0][3] = 0;
		ret.m[1][3] = 0;
		ret.m[2][3] = 0;
		ret.m[3][3] = 1;
		return ret;
	}

	void MathUtils::CalcBinormalTangent(Vector3f normal, Vector3f& binormal, Vector3f& tangent)
	{
		//归一化,精度
		normal.Normalize();
		if (std::abs(normal.x) < C_FLOAT_EPSILON)
			normal.x = 0;
		if (std::abs(normal.y) < C_FLOAT_EPSILON)
			normal.y = 0;
		if (std::abs(normal.z) < C_FLOAT_EPSILON)
			normal.z = 0;
		//phi表示法线在xz平面上的投影于+x轴的夹角
		//theta表示法线与xz平面的夹角
		Float phi = std::atan2(normal.z, normal.x);
		//Float fTheta = std::atan2(normal.y, sqrt(normal.x*normal.x+normal.z*normal.z));

		//s基向量就是将x轴沿y轴顺时针旋转pi/2-phi
		tangent.x = std::sin(phi);
		tangent.y = 0;
		tangent.z = -std::cos(phi);

		//注意方向
		tangent = -tangent;
		binormal = Cross(normal, tangent);

		binormal.Normalize();
		tangent.Normalize();
	}

	void MathUtils::VectorToRotation(const Vector3f& vec, Float& pitch, Float& yaw, Float& roll)
	{
		yaw = (std::atan2(vec.y, vec.x) * (Float)(0xffff) / (C_PI * 2.0));
		pitch = (std::atan2(vec.z, std::sqrt(vec.x * vec.x + vec.y * vec.y)) * (Float)(0xffff) / (C_PI * 2.0));
		roll = 0;
	}

	Vector3f MathUtils::RotateVector(const Quaternion& q, const Vector3f& vec)
	{
		// nVidia SDK implementation
		Vector3f uv, uuv;
		Vector3f qvec(q.x, q.y, q.z);
		uv = Cross(qvec, vec);
		uuv = Cross(qvec, uv);
		uv *= (2.0 * q.w);
		uuv *= 2.0;

		return vec + uv + uuv;
	}

	BoundingBox MathUtils::TransformBoundingBox(const BoundingBox& boundingBox, const Matrix4f& mat)
	{
		if (!boundingBox.IsValid())
		{
			return BoundingBox::INVALID;
		}
		BoundingBox ret;
		ret.min = ret.max = mat.GetTranslation();
		if (mat[0][0] > 0.0f)
		{
			ret.min.x += mat[0][0] * boundingBox.min.x;
			ret.max.x += mat[0][0] * boundingBox.max.x;
		}
		else
		{
			ret.min.x += mat[0][0] * boundingBox.max.x;
			ret.max.x += mat[0][0] * boundingBox.min.x;
		}
		if (mat[0][1] > 0.0f)
		{
			ret.min.y += mat[0][1] * boundingBox.min.x;
			ret.max.y += mat[0][1] * boundingBox.max.x;
		}
		else
		{
			ret.min.y += mat[0][1] * boundingBox.max.x;
			ret.max.y += mat[0][1] * boundingBox.min.x;
		}
		if (mat[0][2] > 0.0f)
		{
			ret.min.z += mat[0][2] * boundingBox.min.x;
			ret.max.z += mat[0][2] * boundingBox.max.x;
		}
		else
		{
			ret.min.z += mat[0][2] * boundingBox.max.x;
			ret.max.z += mat[0][2] * boundingBox.min.x;
		}

		if (mat[1][0] > 0.0f)
		{
			ret.min.x += mat[1][0] * boundingBox.min.y;
			ret.max.x += mat[1][0] * boundingBox.max.y;
		}
		else
		{
			ret.min.x += mat[1][0] * boundingBox.max.y;
			ret.max.x += mat[1][0] * boundingBox.min.y;
		}
		if (mat[1][1] > 0.0f)
		{
			ret.min.y += mat[1][1] * boundingBox.min.y;
			ret.max.y += mat[1][1] * boundingBox.max.y;
		}
		else
		{
			ret.min.y += mat[1][1] * boundingBox.max.y;
			ret.max.y += mat[1][1] * boundingBox.min.y;
		}
		if (mat[1][2] > 0.0f)
		{
			ret.min.z += mat[1][2] * boundingBox.min.y;
			ret.max.z += mat[1][2] * boundingBox.max.y;
		}
		else
		{
			ret.min.z += mat[1][2] * boundingBox.max.y;
			ret.max.z += mat[1][2] * boundingBox.min.y;
		}
		if (mat[2][0] > 0.0f)
		{
			ret.min.x += mat[2][0] * boundingBox.min.z;
			ret.max.x += mat[2][0] * boundingBox.max.z;
		}
		else
		{
			ret.min.x += mat[2][0] * boundingBox.max.z;
			ret.max.x += mat[2][0] * boundingBox.min.z;
		}
		if (mat[2][1] > 0.0f)
		{
			ret.min.y += mat[2][1] * boundingBox.min.z;
			ret.max.y += mat[2][1] * boundingBox.max.z;
		}
		else
		{
			ret.min.y += mat[2][1] * boundingBox.max.z;
			ret.max.y += mat[2][1] * boundingBox.min.z;
		}
		if (mat[2][2] > 0.0f)
		{
			ret.min.z += mat[2][2] * boundingBox.min.z;
			ret.max.z += mat[2][2] * boundingBox.max.z;
		}
		else
		{
			ret.min.z += mat[2][2] * boundingBox.max.z;
			ret.max.z += mat[2][2] * boundingBox.min.z;
		}
		return ret;
	}
}