#pragma once

#include "vector_helper.h"

template<typename T>
class Vector4 {
public:
	/* boring stuff */
	[[nodiscard]] Vector4();
	
	explicit Vector4(T X, T Y, T Z, T W);
	explicit Vector4(const T* XYZW);
	
	Vector4(const Vector4 &other) {
		for (i32 i = 0; i < N; ++i) {
			this->XYZW[i] = other.XYZW[i];
		}
	}
	
	Vector4(Vector4 &&other) noexcept {
		for (i32 i = 0; i < N; ++i) {
			this->XYZW[i] = other.XYZW[i];
		}
	}

	Vector4<T> operator=(const Vector4 &other) {
		for (i32 i = 0; i < N; ++i) {
			this->XYZW[i] = other.XYZW[i];
		}
		return *this;
	}

	Vector4<T> operator=(Vector4 &&other) noexcept {
		for (i32 i = 0; i < N; ++i) {
			this->XYZW[i] = other.XYZW[i];
		}
		return *this;
	}

	Vector4<T> operator=(const T* XYZW) noexcept {
		for (i32 i = 0; i < N; ++i) {
			this->XYZW[i] = XYZW[i];
		}
		return *this;
	}

	Vector4<T> operator*=(const Vector4 &lhs) {
		Vector4 A = *this * lhs;
		*this = A.XYZW;
		return *this;
	}

	friend bool operator==(const Vector4 &lhs, const Vector4 &rhs) {
		return true;
	}

	friend bool operator!=(const Vector4 &lhs, const Vector4 &rhs) {
		return !(lhs == rhs);
	}

	friend Vector4<T> operator*(const Vector4 &lhs, const Vector4 &rhs) {
		T OutXYZW[N];
		for (i32 i = 0; i < N; ++i) {
			OutXYZW[i] = lhs.XYZW[i] * rhs.XYZW[i];
		}
		return Vector4(OutXYZW); 
	}

	/* interesting stuff */
	Vector4& Lerp(const Vector4 &lhs, float t);
	static Vector4 Lerp(const Vector4 &lhs, const Vector4 &rhs, float t);

	static constexpr i32 N = 4;
	T XYZW[N];
};

template <typename T>
Vector4<T>::Vector4() {
}

template <typename T>
Vector4<T>::Vector4(T X, T Y, T Z, T W) {
	this->XYZW[0] = X;
	this->XYZW[1] = Y;
	this->XYZW[2] = Z;
	this->XYZW[3] = W;
}

template <typename T>
Vector4<T>::Vector4(const T* XYZW) {
	for (i32 i = 0; i < N; ++i) {
		this->XYZW[i] = XYZW[i];
	}
}

template <typename T>
Vector4<T>& Vector4<T>::Lerp(const Vector4 &lhs, float t) {
	return Lerp(this->XYZW, lhs, t); 
}

template <typename T>
Vector4<T> Vector4<T>::Lerp(const Vector4 &lhs, const Vector4 &rhs, float t) {
	T OutXYZW[N];
	for (i32 i = 0; i < N; ++i) {
		OutXYZW[i] = MathFunctions::lerp(lhs.XYZW[i], rhs.XYZW[i], t);
	}
	return Vector4(OutXYZW);
}

using FVector = Vector4<f64>;