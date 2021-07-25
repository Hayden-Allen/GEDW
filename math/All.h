#pragma once
#include "Core.h"
#include "Vec2.h"
#include "Range.h"
#include "LinkedListNode.h"
#include "QuadTree.h"
#include "Ray.h"

namespace math
{
	// simple AABB check
	template<typename A, typename B, typename C, typename D>
	static bool rectIntersect(const Vec2<A>& ap, const Vec2<B>& ad, const Vec2<C>& bp, const Vec2<D>& bd, const RangeOverlapsParams& options = {})
	{
		const Range ax(ap.x, ap.x + ad.x), ay(ap.y, ap.y + ad.y);
		const Range bx(bp.x, bp.x + bd.x), by(bp.y, bp.y + bd.y);
		return ax.Overlaps(bx, options) && ay.Overlaps(by, options);
	}
}
