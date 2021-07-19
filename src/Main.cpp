#include "pch.h"
#include "graphics/Sprite.h"
#include "graphics/SpriteBank.h"
#include "graphics/Renderer.h"
#include "world/Map.h"
#include "world/dynamic/Dynamic.h"
#include "world/dynamic/DynamicList.h"

using namespace engine;

struct Rect
{
	math::Vec2<float> pos = { 0.f, 0.f }, dim = { 0.f, 0.f };
};
static bool rayrect(const math::Vec2<float>& origin, const math::Vec2<float>& direction, const Rect& rect, math::Vec2<float>* const normal, math::Vec2<float>* const contact, float* const time)
{
	// near and far "times" at which our ray intersects with the given rect (remember, "time" is the distance along our given ray at which the intersection happens)
	math::Vec2<float> near = (rect.pos - origin) / direction;
	math::Vec2<float> far = (rect.pos + rect.dim - origin) / direction;

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

int main()
{
	EngineInstance engine = init(800, 600, "ACM Game Engine Dev Workshop Series", { .resizable = true, .pixelSize = 2.f, .clear = {.b = 1.f } });
	Renderer renderer("res/shader_texture.glsl", "res/shader_dynamic.glsl", &engine);


	SpriteBank sb;
	Map map("res/map.txt", sb, engine);


	DynamicList dl;
	Sprite* dsprite = sb.Put("res/test2.bmp", 2, 500);
	Dynamic* dynamic = new Dynamic(dl, { -16.f, -16.f }, { 0.f, 0.f }, 100.f, { {"a", dsprite} }, "a");


	// generate a 100x100 grid of hitboxes starting at the origin
	constexpr uint tileWidth = 100, tileHeight = 100, tileCount = tileWidth * tileHeight;
	const math::Vec2<float> size = dsprite->GetDims() * 2.f;
	Rect boxes[tileCount];
	for (uint i = 0; i < tileCount; i++)
	{
		const uint x = i % tileWidth, y = i / tileWidth;
		math::Vec2<float> pos = { 1.f * x, 1.f * y };
		// artificially inflate each hitbox to account for our player's dimensions
		pos *= size / 2.f;
		pos -= size / 4.f;
		boxes[i] = { pos, size };
	}


	math::Vec2<float> camera = { 0.f, 0.f };
	const float camSpeed = 100.f;
	// game loop
	while (engine.IsRunning())
	{
		// clear the screen at the start of each frame
		renderer.Clear();

		// move on x-axis according to A and D and y-axis according to W and S
		math::Vec2<float> offset =
		{
			1.f * engine.IsKeyPressed('A') - engine.IsKeyPressed('D'),
			1.f * engine.IsKeyPressed('S') - engine.IsKeyPressed('W')
		};
		// if we are moving along both axes, scale appropriately to maintain a magnitude of `camSpeed`
		if (!math::isZero(offset.x) && !math::isZero(offset.y))
			offset /= math::sqrt(2.f);


		// set our player's velocity according to the keyboard
		dynamic->SetVel(offset * -dynamic->GetSpeed());
		// cast a ray from our current position to the position we want to end up this frame
		const math::Vec2<float> origin = dynamic->GetPos(), direction = dynamic->GetVel() * renderer.GetFrameDelta();

		// store info about collisions
		math::Vec2<float> normal = { 0.f, 0.f }, contact = normal;
		float time = 0.f;
		// check our player against each rect
		for (const Rect& box : boxes)
		{
			// if we intersect with the current rect, fix it
			if (rayrect(origin, direction, box, &normal, &contact, &time))
			{
				auto vel = dynamic->GetVel();
				vel += normal * vel.Abs() * (1 - time);
				dynamic->SetVel(vel);
			}
		}
		// make our camera track our player
		renderer.SetCamera(-dynamic->GetPos());
		// draw everything
		map.Draw(renderer);
		dl.Draw(renderer);

		renderer.Render();
	}

	// clean up
	end(engine);
	return 0;
}