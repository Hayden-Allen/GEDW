#pragma once
#include <cmath>
#include <inttypes.h>
#include <stdio.h>
#include <string>

// rename these for convenience
typedef uint8_t uchar;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef uint64_t ulong;

// cast x to a compatible type T
#define CAST(T, x) static_cast<T>(x)
// cast x to type T
#define PUN(T, x) *(T*)&x

namespace math
{
	// maximum value considered to be "zero"
	static float EPSILON = .0001f;
	constexpr static float PI = 3.14159265359f, E = 2.71828182846f;


	// |t|
	template<typename T>
	static T abs(const T& t)
	{
		return t < 0 ? -t : t;
	}
	template<typename A, typename B>
	static A min(const A& a, const B& b)
	{
		return a < b ? a : CAST(A, b);
	}
	template<typename A, typename B>
	static A max(const A& a, const B& b)
	{
		return a > b ? a : CAST(A, b);
	}
	// returns a if it is in [lo, hi]. Otherwise, returns
	// whichever of lo and hi is closest to a.
	template<typename A, typename B, typename C>
	static A clamp(const A& a, const B& lo, const C& hi)
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
	static void setEpsilon(float f)
	{
		EPSILON = f;
	}
	// account for negative values by using |t|
	template<typename T>
	static bool isZero(const T& t)
	{
		return abs(t) <= EPSILON;
	}


	// These were previously totally messed up.
	// They both returned bools (?) and I flipped the names.
	template<typename T>
	static float deg(const T& t)
	{
		return t * 180 / PI;
	}
	template<typename T>
	static float rad(const T& t)
	{
		return t * PI / 180;
	}


	// number of elements in a compile-time array
	template<typename T, uint N>
	static uint arrlen(const T(&arr)[N])
	{
		return N;
	}
	// size in bytes of a compile-time array
	template<typename T, uint N>
	static uint arrsize(const T(&arr)[N])
	{
		return N * sizeof(T);
	}
	// converts each character in s to lowercase
	static void toLower(std::string& s)
	{
		for (uint i = 0; i < s.size(); i++)
			s[i] = tolower(s[i]);
	}
	// returns toLower(a) == toLower(b)
	static bool equalsIgnoreCase(const std::string& a, const std::string& b)
	{
		if (a.size() != b.size())
			return false;
		for (uint i = 0; i < a.size(); i++)
			if (tolower(a[i]) != tolower(b[i]))
				return false;
		return true;
	}
	template<typename T>
	static void arrprint(uint count, T* arr, const std::string& fmt, const std::string& sep, uint wrap = 10)
	{
		printf("[\n\t");
		const uint bufferLength = CAST(uint, fmt.length() + sep.length() + 2);
		char* buffer = new char[bufferLength];
		for (uint i = 0; i < count; i++)
		{
			sprintf_s(buffer, bufferLength, "%s%s", fmt.c_str(), (i != count - 1 ? sep.c_str() : ""));
			printf(buffer, arr[i]);
			if (i != 0 && (i + 1) % wrap == 0)
				printf("\n\t");
		}
		printf("\n]\n");
		delete[] buffer;
	}
	template<typename T, uint N>
	static void arrprint(T(&arr)[N], const std::string& fmt, const std::string& sep, uint wrap = 10)
	{
		arrprint(N, arr, fmt, sep, wrap);
	}
	template<typename T>
	static int sign(T t)
	{
		return (t == 0 ? 0 : (t > 0 ? 1 : -1));
	}
}
