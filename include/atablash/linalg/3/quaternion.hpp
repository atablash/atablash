#pragma once

#include <atablash/math.hpp>

namespace ab{

template<typename TFLOAT>
class Quaternion
{
public:
	Quaternion() {}

	Quaternion(const TFLOAT& _x, const TFLOAT& _y, const TFLOAT& _z, const TFLOAT& _w) : x(_x), y(_y), z(_z), w(_w) {}

	explicit Quaternion(const Vector3<TFLOAT> v){
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0.0;
	}

	template<class G>
	explicit Quaternion(const Quaternion<G>& o){
		x = (TFLOAT)o.x;
		y = (TFLOAT)o.y;
		z = (TFLOAT)o.z;
		w = (TFLOAT)o.w;
	}

	operator std::string()const{
		return "{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + "}";
	}

	operator Vector3<TFLOAT>(){
		return Vector3<TFLOAT>(x,y,z);
	}

	// yaw,pitch,roll: looking along x axis, z up (from wikipedia); not tested
	static Quaternion QuaternionZYX(const TFLOAT& yawZ, const TFLOAT& pitchY, const TFLOAT& rollX){
		Quaternion result;

		TFLOAT pitch2 = pitchY * 0.5;
		TFLOAT yaw2 = yawZ * 0.5;
		TFLOAT roll2 = rollX * 0.5;

		TFLOAT sp = sin(pitch2);
		TFLOAT sy = sin(yaw2);
		TFLOAT sr = sin(roll2);

		TFLOAT cp = cos(pitch2);
		TFLOAT cy = cos(yaw2);
		TFLOAT cr = cos(roll2);

		result.w = cr*cp*cy + sr*sp*sy;
		result.x = sr*cp*cy - cr*sp*sy;
		result.y = cr*sp*cy + sr*cp*sy;
		result.z = cr*cp*sy - sr*sp*cy;

		return result;
	}

	Quaternion(const Vector3<TFLOAT> axis, TFLOAT angle){
		const TFLOAT eps = (TFLOAT) 0.0000001;
		// TODO: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		while (angle >  PI) angle -= (TFLOAT)(2 * PI);
		while (angle < -PI) angle += (TFLOAT)(2 * PI);

		ASS(angle <= PI + eps);
		ASS(angle >= -PI-eps);
		TFLOAT angle2 = angle * 0.5f;
		TFLOAT s = sin(angle2);

		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		w = cos(angle2);

		ASS(w >= -eps);
	}

	static Quaternion<TFLOAT> RotX(TFLOAT angle){
		return Quaternion<TFLOAT>{sin(angle*(TFLOAT)0.5), 0, 0, cos(angle*(TFLOAT)0.5)};
	}

	static Quaternion<TFLOAT> RotY(TFLOAT angle){
		return Quaternion<TFLOAT>{0, sin(angle*(TFLOAT)0.5), 0, cos(angle*(TFLOAT)0.5)};
	}

	static Quaternion<TFLOAT> RotZ(TFLOAT angle){
		return Quaternion<TFLOAT>{0, 0, sin(angle*(TFLOAT)0.5), cos(angle*(TFLOAT)0.5)};
	}

	TFLOAT getYaw() const{
		return (TFLOAT)atan2(2.0f*(w*z + x*y), 1.0f - 2.0f*(y*y + z*z));
	}

	TFLOAT getAngle() const{
		return (TFLOAT)(acos(w)*2.0);
	}


	/*
	Quaternion& operator*=(const TFLOAT& a){
		x *= a;
		y *= a;
		z *= a;
		w *= a;
		return *this;
	}

	Quaternion& operator/=(const TFLOAT& a){
		const TFLOAT div = 1.0f / a;
		//ASS_TFLOAT(div);
		x *= div;
		y *= div;
		z *= div;
		w *= div;
		return *this;
	}
	*/

	TFLOAT sqrMagnitude() const{
		return x*x + y*y + z*z + w*w;
	}

	TFLOAT magnitude() const{
		TFLOAT normSqr = x*x + y*y + z*z + w*w;
		ASS(normSqr >= 0);
		return sqrt(normSqr);
	}

	Quaternion& normalize(){
		const TFLOAT normSqr = x*x + y*y + z*z + w*w;
		ASS(normSqr >= 0);
		const TFLOAT norm = sqrt(normSqr);
		//ASS_TFLOAT(norm);
		const TFLOAT div = 1.0f / norm;
		x *= div;
		y *= div;
		z *= div;
		w *= div;
		return *this;
	}

	Quaternion reciprocal() const{
		Quaternion result(*this);
		result.reciprocate();
		return result;
	}

	Quaternion& reciprocate(){
		const TFLOAT nsqrinv = 1.0f / (x*x + y*y + z*z + w*w);

		//ASS_TFLOAT(nsqrinv);

		w *= nsqrinv;
		x *= -nsqrinv;
		y *= -nsqrinv;
		z *= -nsqrinv;
		return *this;
	}

	Quaternion conjugated() const{
		Quaternion result(*this);
		result.conjugate();
		return result;
	}

	Quaternion& conjugate(){
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	Quaternion operator-() const {
		return conjugated();
	}

	Quaternion& power(const TFLOAT& exp){
		//ASS_TFLOAT(x);
		//ASS_TFLOAT(y);
		//ASS_TFLOAT(z);
		//ASS_TFLOAT(w);
		//ASS_TFLOAT(exp);

		const TFLOAT d = x*x + y*y + z*z;
		if (d<0.000001) return *this;		// probably not enough?

		//ASS(w <=  1.0f);		// triggers! // at least was true in java
		ASS(w >= -1.0f);

		if (w > 1.0f) w = 1.0f;

		w = cos((TFLOAT)(acos(w)*exp));
		//ASS_TFLOAT(w);
		TFLOAT n = 1.0f - w*w;
		ASS(n >= 0.0f);
		TFLOAT scale = sqrt(n / d);

		ASS(w <= 1.0f);
		ASS(w >= -1.0f);

		//ASS_TFLOAT(scale);

		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, const TFLOAT& t){
		//ASS_TFLOAT(q1.x);
		//ASS_TFLOAT(q1.y);
		//ASS_TFLOAT(q1.z);
		//ASS_TFLOAT(q1.w);
		//ASS_TFLOAT(q2.x);
		//ASS_TFLOAT(q2.y);
		//ASS_TFLOAT(q2.z);
		//ASS_TFLOAT(q2.w);

		ASS(q1.w <= 1.0f);
		ASS(q1.w >= -1.0f);

		ASS(q2.w <= 1.0f);
		ASS(q2.w >= -1.0f);

		//if(q1.w < -1.0) q1.w = -1.0;

		return (q1 * q1.conjugated() * q2).Fix().Power(t);	// Fix ensures that we interpolate shorter way
	}


//private:
	// Make rotation [-pi..pi]
	Quaternion& fix(){
		if (w < 0){
			x = -x;
			y = -y;
			z = -z;
			w = -w;
		}
		return *this;
	}

public:
	TFLOAT x = 0;
	TFLOAT y = 0;
	TFLOAT z = 0;
	TFLOAT w = (TFLOAT)1;
};




template<typename TFLOAT>
Quaternion<TFLOAT> operator*(const Quaternion<TFLOAT>& a, const Quaternion<TFLOAT>& b){
	//ASS(w >= 0);
	//ASS(o.w >= 0);
	//ASS(w*o.w - x*o.x - y*o.y - z*o.z >= 0);		// triggers!	// from java

	return Quaternion<TFLOAT>(
		a.x*b.w + a.w*b.x + a.y*b.z - a.z*b.y,
		a.y*b.w + a.w*b.y + a.z*b.x - a.x*b.z,
		a.z*b.w + a.w*b.z + a.x*b.y - a.y*b.x,
		a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z);
}


// transform vector (not regular multiplication!)
template<class F>
inline Vector3<F> operator*(const Quaternion<F>& quat, const Vector3<F>& vec){
	//ASS(quat.w >= 0);
	return Vector3<F>(quat * Quaternion<F>(vec) * quat.conjugated());
}
template<class F>
inline Vector3<F>& operator*=(Vector3<F>& vec, const Quaternion<F>& quat){
	vec = quat * vec; // can't really benefit from *=
}




// abbrev
typedef ab::Quaternion<float>  quatf;
typedef ab::Quaternion<double> quatd;



}






