#pragma once

namespace math
{
	template<typename T>
	struct Vec2
	{
		T x, y;


		Vec2() : x(CAST(T, 0)), y(CAST(T, 0)) {}
		template<typename U>
		Vec2(const U& u) : x(CAST(T, u)), y(CAST(T, u)) {}
		Vec2(const T& _x, const T& _y) : x(_x), y(_y) {}
		// copy constructor
		Vec2(const Vec2<T>& other)
		{
			operator=(other);
		}
		// move constructor (compiler wants "noexcept" even though it's irrelevant to us)
		Vec2(Vec2<T>&& other) noexcept
		{
			operator=(other);
		}


		bool operator==(const T& t) const
		{
			return x == t && y == t;
		}
		// short circuit if x != other.x
		bool operator==(const Vec2<T>& other) const
		{
			return isZero(x - other.x) && isZero(y - other.y);
		}
		// short circuit if x == other.x
		bool operator!=(const Vec2<T>& other) const
		{
			return !isZero(x - other.x) || !isZero(y - other.y);
		}
		// copy values from other into this
		Vec2<T>& operator=(const Vec2<T>& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}
		// return new vector with both of this vector's fields PLUS t
		Vec2<T> operator+(const T& t) const
		{
			return { x + t, y + t };
		}
		// return new vector with this vector's fields PLUS other's fields
		Vec2<T> operator+(const Vec2<T>& other) const
		{
			return { x + other.x, y + other.y };
		}
		// add t to both of this vector's fields
		Vec2<T>& operator+=(const T& t)
		{
			x += t;
			y += t;
			return *this;
		}
		// add other's fields to this vector's fields
		Vec2<T>& operator+=(const Vec2<T>& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Vec2<T> operator-() const
		{
			return { -x, -y };
		}
		// return new vector with both of this vector's fields MINUS t
		Vec2<T> operator-(const T& t) const
		{
			return { x - t, y - t };
		}
		// return new vector with this vector's fields MINUS other's fields
		Vec2<T> operator-(const Vec2<T>& other) const
		{
			return { x - other.x, y - other.y };
		}
		// subtract t from both of this vector's fields
		Vec2<T>& operator-=(const T& t)
		{
			x -= t;
			y -= t;
			return *this;
		}
		// subtract other's fields from this vector's fields
		Vec2<T>& operator-=(const Vec2<T>& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		// return new vector with both of this vector's fields TIMES t
		Vec2<T> operator*(const T& t) const
		{
			return { x * t, y * t };
		}
		// return new vector with this vector's fields TIMES other's fields
		Vec2<T> operator*(const Vec2<T>& other) const
		{
			return { x * other.x, y * other.y };
		}
		// MULTIPLY this vector's fields by t
		Vec2<T>& operator*=(const T& t)
		{
			x *= t;
			y *= t;
			return *this;
		}
		// MULTIPLY this vector's fields by other's fields
		Vec2<T>& operator*=(const Vec2<T>& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		// return new vector with both of this vector's fields DIVIDED BY t
		Vec2<T> operator/(const T& t) const
		{
			return { x / t, y / t };
		}
		// return new vector with this vector's fields DIVIDED BY other's fields
		Vec2<T> operator/(const Vec2<T>& other) const
		{
			return { x / other.x, y / other.y };
		}
		// DIVIDE this vector's fields by t
		Vec2<T>& operator/=(const T& t)
		{
			x /= t;
			y /= t;
			return *this;
		}
		// DIVIDE this vector's fields by other's fields
		Vec2<T>& operator/=(const Vec2<T>& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
		// the length of this vector
		T Magnitude() const
		{
			return dist(0.f, 0.f, x, y);
		}
		// return a new vector that is this vector normalized
		Vec2<T> Normalized() const
		{
			return operator/(Magnitude());
		}
		// normalize this vector and return it
		Vec2<T>& Normalize()
		{
			return operator/=(Magnitude());
		}
		// dot product of this vector and other
		T Dot(const Vec2<T>& other) const
		{
			return x * other.x + y * other.y;
		}
		// the angle (relative to the x-axis) of this vector
		T Angle() const
		{
			return atan(x, y);
		}
		// the angle between this vector and other
		T AngleBetween(const Vec2<T>& other) const
		{
			return acos(Dot(other) / (Magnitude() * other.Magnitude()));
		}
		Vec2<T>& Clamp(const T& lo, const T& hi)
		{
			const T mag = Magnitude();
			if (mag < lo)
				return Normalize() *= lo;
			if (mag > hi)
				return Normalize() *= hi;
			return *this;
		}
		Vec2<T> Unit() const
		{
			return { CAST(T, sign(x)), CAST(T, sign(y)) };
		}
		Vec2<T> Abs() const
		{
			return { abs(x), abs(y) };
		}
		bool IsZero() const
		{
			return isZero(x) && isZero(y);
		}
	};



	template<typename T>
	Vec2<T> operator+(const T& t, const Vec2<T>& v)
	{
		return { t + v.x, t + v.y };
	}
	template<typename T>
	Vec2<T> operator-(const T& t, const Vec2<T>& v)
	{
		return { t - v.x, t - v.y };
	}
	template<typename T>
	Vec2<T> operator*(const T& t, const Vec2<T>& v)
	{
		return { t * v.x, t * v.y };
	}
	template<typename T>
	Vec2<T> operator/(const T& t, const Vec2<T>& v)
	{
		return { t / v.x, t / v.y };
	}
}
