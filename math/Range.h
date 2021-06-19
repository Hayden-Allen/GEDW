#pragma once

namespace math
{
	struct RangeContainsParams
	{
		bool left = false, right = false;
	};
	struct RangeOverlapParams
	{
		RangeContainsParams left = {}, right = {};
	};

	template<typename T>
	class Range
	{
	public:
		Range(const T& min, const T& max) :
			m_Min(min),
			m_Max(max)
		{
			if (min > max)
				printf("Range min must be less than max\n");
		}
		Range(const Range<T>& other) { operator=(other); }
		Range(Range<T>&& other) { operator=(other); }


		bool operator==(const Range<T>& other) const
		{
			return isZero(m_Min - other.m_Min) && isZero(m_Max - other.m_Max);
		}
		bool operator!=(const Range<T>& other) const
		{
			return !isZero(m_Min - other.m_Min) || !isZero(m_Max - other.m_Max);
		}
		// Ranges are immutable so we return a new one here
		Range<T> operator+(const T& t) const
		{
			return Range<T>(m_Min + t, m_Max + t);
		}
		// params controls how t is evaluated for containment.
		// By default, this check is exclusive at the boundaries.
		// If params.left is true, m_Min is included in the check and if params.right is true m_Max is included in the check. That is, to make this check completely inclusive, set both params.left and params.right to true.
		bool Contains(const T& t, const RangeContainsParams& params = {}) const
		{
			return (params.left ? m_Min <= t : m_Min < t) &&
				(params.right ? t <= m_Max : t < m_Max);
		}
		// params.left controls how other.m_Min is evaluated for containment
		// params.right controls how other.m_Max is evaluated for containment
		bool Overlaps(const Range<T>& other, const RangeOverlapParams& params = {}) const
		{
			return Contains(other.m_Min, params.left) || Contains(other.m_Max, params.right);
		}
		const T& GetMin() const
		{
			return m_Min;
		}
		const T& GetMax() const
		{
			return m_Max;
		}
		// Return values for the three cases:
		//	- (t < m_Min)				 returns m_Min
		//	- (t > m_Max)				 returns m_Max
		//	- (t >= m_Min && t <= m_Max) returns t
		T Clamp(const T& t) const
		{
			return clamp(t, m_Min, m_Max);
		}
		T GetSpan() const
		{
			return m_Max - m_Min;
		}
	private:
		T m_Min, m_Max;
	};
}
