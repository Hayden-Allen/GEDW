#pragma once

namespace math
{
	template<typename O, typename D = O>
	struct Ray
	{
		Vec2<O> origin;
		Vec2<D> direction;


		Ray(const Vec2<O>& o, const Vec2<D>& d) :
			origin(o),
			direction(d)
		{}


		bool IntersectsRect(const Vec2<float>& pos, const Vec2<float>& dim, math::Vec2<float>* const normal, math::Vec2<float>* const contact, float* const time)
		{
			// near and far "times" at which our ray intersects with the given rect (remember, "time" is the distance along our given ray at which the intersection happens)
			math::Vec2<float> near = (pos - origin) / direction;
			math::Vec2<float> far = (pos + dim - origin) / direction;

			// if the order is messed up, fix it
			if (far.x < near.x)
				std::swap(near.x, far.x);
			if (far.y < near.y)
				std::swap(near.y, far.y);

			// no intersection
			if (!(near.x < far.y && near.y < far.x))
				return false;

			// "time" of first intersection
			float t = math::max(near.x, near.y);
			// if the intersection occurs either backwards in "time" or further in the future than we care about this frame, just say there's no intersection
			if (t < 0 || t > 1)
				return false;

			if (time)
				*time = t;

			// (x, y) coordinate at which our ray intersects the rect
			if (contact)
				*contact = origin + direction * t;

			// hacky math to simplify an if/else chain to determine which face we're intersecting with
			bool xfirst = near.x < near.y;
			if (normal)
				*normal = math::Vec2<float>(1.f * !xfirst, 1.f * xfirst) * -direction.Unit();

			return true;
		}
	};
}
