#include "pch.h"
#include "graphics/Sprite.h"
#include "graphics/SpriteBank.h"
#include "graphics/Renderer.h"
#include "world/Map.h"
#include "world/dynamic/Dynamic.h"
#include "world/dynamic/DynamicList.h"

using namespace engine;

int main()
{
	EngineInstance engine = init(800, 600, "ACM Game Engine Dev Workshop Series", { .resizable = true, .pixelSize = 2.f, .clear = {.b = 1.f } });
	Renderer renderer("res/shader_texture.glsl", "res/shader_dynamic.glsl", &engine);


	SpriteBank sb;
	Map map("res/map.txt", sb, engine);


	DynamicList dl;
	Sprite* dsprite = sb.Put("res/test2.bmp", 2, 500);
	Sprite* dsprite2 = sb.Put("res/test3.bmp", 1, 0);
	Dynamic* dynamic = new Dynamic(dl, { 0.f, 0.f }, { 0.f, 0.f }, 100.f, { {"a", dsprite} }, "a");

	// create 100 new dynamics
	for (uint i = 0; i < 100; i++)
	{
		Dynamic* t = new Dynamic(dl, { 1.f * i * dsprite2->GetWidth(), 1.f * i * dsprite2->GetHeight() }, { 0.f, 0.f }, 0.f, { {"a", dsprite2} }, "a");
	}
	// delete some of them (to test that deletion works)
	for (uint i = 0; i < 75; i++)
	{
		Dynamic* d = dl[dl.GetLast() - 1 - i];
		dl.Remove(d);
		delete d;
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

		camera += offset * camSpeed * renderer.GetFrameDelta();
		// update camera uniform
		renderer.SetCamera(camera);

		map.Draw(renderer);

		dynamic->SetVel(offset * -dynamic->GetSpeed());
		dl.Draw(renderer);

		renderer.Render();
	}

	// clean up
	end(engine);
	return 0;
}