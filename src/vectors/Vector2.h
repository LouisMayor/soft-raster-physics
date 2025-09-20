#pragma once
#include "vector_helper.h"
#include <cmath>

template <typename T> class Vector2 {
  public:
	/* boring stuff */
	[[nodiscard]] Vector2();

	explicit Vector2(
		T X,
		T Y
	);
	explicit Vector2(const T *XY);

	Vector2(
		const Vector2 &other
	) {
		for (i32 i = 0; i < N; ++i) {
			this->XY[i] = other.XY[i];
		}
	}

	Vector2(
		Vector2 &&other
	) noexcept {
		for (i32 i = 0; i < N; ++i) {
			this->XY[i] = other.XY[i];
		}
	}

	Vector2<T> operator=(
		const Vector2 &other
	) {
		for (i32 i = 0; i < N; ++i) {
			this->XY[i] = other.XY[i];
		}
		return *this;
	}

	Vector2<T> operator=(
		Vector2 &&other
	) noexcept {
		for (i32 i = 0; i < N; ++i) {
			this->XY[i] = other.XY[i];
		}
		return *this;
	}

	Vector2<T> operator=(
		const T *XY
	) noexcept {
		for (i32 i = 0; i < N; ++i) {
			this->XY[i] = XY[i];
		}
		return *this;
	}

	Vector2<T> operator*=(
		const Vector2 &lhs
	) {
		Vector2 A = *this * lhs;
		*this = A.XY;
		return *this;
	}

	Vector2<T> operator*=(
		const f64 &lhs
	) {
		Vector2 A = *this * lhs;
		*this = A.XY;
		return *this;
	}

	Vector2<T> operator+=(
		const Vector2 &lhs
	) {
		Vector2 A = *this + lhs;
		*this = A.XY;
		return *this;
	}

	friend bool operator==(
		const Vector2 &lhs,
		const Vector2 &rhs
	) {
		return lhs.XY[0] == rhs.XY[0] && lhs.XY[1] == rhs.XY[1];
	}

	friend bool operator!=(
		const Vector2 &lhs,
		const Vector2 &rhs
	) {
		return !(lhs == rhs);
	}

	friend Vector2<T> operator*(
		const Vector2 &lhs,
		const Vector2 &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs.XY[i] * rhs.XY[i];
		}
		return Vector2(OutXY);
	}

	friend Vector2<T> operator*(
		const Vector2 &lhs,
		const f64 &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs.XY[i] * rhs;
		}
		return Vector2(OutXY);
	}

	friend Vector2<T> operator*(
		const f64 &lhs,
		const Vector2 &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs * rhs.XY[i];
		}
		return Vector2(OutXY);
	}

	Vector2<f64> operator*(const Vector2<i32> &lhs);

	friend Vector2<T> operator*(
		const Vector2 &lhs,
		const float &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs.XY[i] * rhs;
		}
		return Vector2(OutXY);
	}

	friend Vector2<T> operator+(
		const Vector2 &lhs,
		const Vector2 &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs.XY[i] + rhs.XY[i];
		}
		return Vector2(OutXY);
	}

	Vector2<f64> operator+(const Vector2<i32> &lhs);

	friend Vector2<T> operator-(
		const Vector2 &lhs,
		const Vector2 &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs.XY[i] - rhs.XY[i];
		}
		return Vector2(OutXY);
	}

	Vector2<T> operator-(const Vector2<T> &lhs);
	Vector2<f64> operator-() const;

	friend Vector2<T> operator/(
		const Vector2 &lhs,
		const Vector2 &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs.XY[i] / rhs.XY[i];
		}
		return Vector2(OutXY);
	}

	friend Vector2<T> operator/(
		const Vector2 &lhs,
		const float &rhs
	) {
		T OutXY[N];
		for (i32 i = 0; i < N; ++i) {
			OutXY[i] = lhs.XY[i] / rhs;
		}
		return Vector2(OutXY);
	}

	/* interesting stuff */
	[[nodiscard]] Vector2 &Lerp(
		const Vector2 &lhs,
		float t
	) const;
	[[nodiscard]] static Vector2 Lerp(
		const Vector2 &lhs,
		const Vector2 &rhs,
		float t
	);

	[[nodiscard]] f64 Dot(const Vector2 &lhs) const;
	[[nodiscard]] static f64
	Dot(const Vector2 &lhs,
		const Vector2 &rhs);

	[[nodiscard]] static f64 Distance(
		const Vector2 &lhs,
		const Vector2 &rhs
	);

	[[nodiscard]] f64 Length() const;
	[[nodiscard]] Vector2 Normalise() const;

	[[nodiscard]] Vector2 Sqrt() const;
	[[nodiscard]] Vector2 Abs() const;

	static constexpr i32 N = 2;
	T XY[2];
};

template <>
inline Vector2<f64> Vector2<f64>::operator*(
	const Vector2<i32> &lhs
) {
	constexpr i32 N = Vector2<f64>::N;
	f64 OutXY[N];
	for (i32 i = 0; i < N; ++i) {
		OutXY[i] = this->XY[i] * lhs.XY[i];
	}
	return Vector2(OutXY);
}

template <>
inline Vector2<f64> Vector2<f64>::operator+(
	const Vector2<i32> &lhs
) {
	constexpr i32 N = Vector2<f64>::N;
	f64 OutXY[N];
	for (i32 i = 0; i < N; ++i) {
		OutXY[i] = this->XY[i] + lhs.XY[i];
	}
	return Vector2(OutXY);
}

template <typename T>
Vector2<T> Vector2<T>::operator-(
	const Vector2<T> &lhs
) {
	T OutXY[N];
	for (i32 i = 0; i < N; ++i) {
		OutXY[i] = this->XY[i] - lhs.XY[i];
	}
	return Vector2(OutXY);
}

template <typename T> Vector2<f64> Vector2<T>::operator-() const {
	f64 OutXY[N];
	for (i32 i = 0; i < N; ++i) {
		OutXY[i] = -this->XY[i];
	}
	return Vector2(OutXY);
}

template <typename T> Vector2<T>::Vector2() {
}

template <typename T>
Vector2<T>::Vector2(
	T X,
	T Y
) {
	this->XY[0] = X;
	this->XY[1] = Y;
}

template <typename T>
Vector2<T>::Vector2(
	const T *XY
) {
	for (i32 i = 0; i < N; ++i) {
		this->XY[i] = XY[i];
	}
}

template <typename T>
Vector2<T> &Vector2<T>::Lerp(
	const Vector2 &lhs,
	float t
) const {
	return Lerp(this->XY, lhs, t);
}

template <typename T>
Vector2<T> Vector2<T>::Lerp(
	const Vector2 &lhs,
	const Vector2 &rhs,
	float t
) {
	T OutXY[N];
	for (i32 i = 0; i < N; ++i) {
		OutXY[i] = MathFunctions::lerp(lhs.XY[i], rhs.XY[i], t);
	}
	return Vector2(OutXY);
}

template <typename T>
f64 Vector2<T>::Dot(
	const Vector2 &lhs
) const {
	return Dot(this->XY, lhs);
}

template <typename T>
f64 Vector2<T>::Dot(
	const Vector2 &lhs,
	const Vector2 &rhs
) {
	f64 d = 0.0;
	for (i32 i = 0; i < N; ++i) {
		d += lhs.XY[i] * rhs.XY[i];
	}
	return d;
}

template <typename T>
f64 Vector2<T>::Distance(
	const Vector2 &lhs,
	const Vector2 &rhs
) {
	f64 d = 0.0;
	for (i32 i = 0; i < N; ++i) {
		d += std::fabs(lhs.XY[i] - rhs.XY[i]);
	}
	return std::fabs(d);
}

template <typename T> f64 Vector2<T>::Length() const {
	return static_cast<T>(std::sqrt(Dot(*this, *this)));
}

template <typename T> Vector2<T> Vector2<T>::Normalise() const {
	return Vector2(*this / this->Length());
}

template <typename T> Vector2<T> Vector2<T>::Sqrt() const {
	return Vector2{sqrt(fabs(this->XY[0])), sqrt(fabs(this->XY[1]))};
}

template <typename T> Vector2<T> Vector2<T>::Abs() const {
	return Vector2{std::abs(XY[0]), std::abs(XY[1])};
}

using FVector2 = Vector2<f64>;
using IVector2 = Vector2<i32>;
