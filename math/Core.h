#pragma once
#include <cmath>
#include <inttypes.h>
#include <stdio.h>
#include <string>

typedef uint8_t uchar;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef uint64_t ulong;

#define CAST(T, x) static_cast<T>(x)
#define PUN(T, x) *(T*)&x

namespace math
{
	constexpr static float PI = 3.14159265359f, E = 2.71828182846f;


	template<typename T>
	static T abs(const T& t)
	{
		return t < 0 ? -t : t;
	}
	template<typename A, typename B>
	static const A& min(const A& a, const B& b)
	{
		return a < b ? a : b;
	}
	template<typename A, typename B>
	static const A& max(const A& a, const B& b)
	{
		return a > b ? a : b;
	}
	template<typename A, typename B, typename C>
	static const A& clamp(const A& a, const B& lo, const C& hi)
	{
		return max(min(a, hi), lo);
	}
	template<typename T>
	static T sqrt(const T& t)
	{
		return std::sqrt(t);
	}

	template<typename T>
	static T sin(const T& t)
	{
		return std::sin(t);
	}
	template<typename T>
	static T cos(const T& t)
	{
		return std::cos(t);
	}
	template<typename T>
	static T tan(const T& t)
	{
		return std::tan(t);
	}

	template<typename T>
	static T asin(const T& t)
	{
		return std::asin(t);
	}
	template<typename T>
	static T acos(const T& t)
	{
		return std::acos(t);
	}
	template<typename X, typename Y>
	static X atan(const X& x, const Y& y)
	{
		return std::atan2(y, x);
	}
	template<typename A, typename B, typename C, typename D>
	static A dist(const A& x1, const B& y1, const C& x2, const D& y2)
	{
		const A dx = x1 - x2;
		const B dy = y1 - y2;
		return sqrt(dx * dx + dy * dy);
	}


	static float EPSILON = .0001f;
	static void setEpsilon(float f)
	{
		EPSILON = f;
	}
	template<typename T>
	static bool isZero(const T& t)
	{
		return abs(t) <= EPSILON;
	}

	template<typename T>
	static bool rad(const T& t)
	{
		return t * 180 / PI;
	}
	template<typename T>
	static bool deg(const T& t)
	{
		return t * PI / 180;
	}
	template<typename T, uint N>
	static uint arrlen(const T(&arr)[N])
	{
		return N;
	}
	template<typename T, uint N>
	static uint arrsize(const T(&arr)[N])
	{
		return N * sizeof(T);
	}
	static void toLower(std::string& s)
	{
		for (uint i = 0; i < s.size(); i++)
			s[i] = tolower(s[i]);
	}
	static bool equalsIgnoreCase(const std::string& a, const std::string& b)
	{
		if (a.size() != b.size())
			return false;
		for (uint i = 0; i < a.size(); i++)
			if (tolower(a[i]) != tolower(b[i]))
				return false;
		return true;
	}
}
