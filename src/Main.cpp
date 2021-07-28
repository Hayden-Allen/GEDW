#include "pch.h"
#include "graphics/Sprite.h"
#include "graphics/SpriteBank.h"
#include "graphics/Renderer.h"
#include "world/Map.h"
#include "world/dynamic/Dynamic.h"
#include "world/dynamic/DynamicList.h"
#include "world/Camera.h"
#include "world/World.h"

using namespace engine;

int main()
{
	EngineInstance engine = init(800, 600, "ACM Game Engine Dev Workshop Series", { .resizable = true, .pixelSize = 2.f, .clear = {.b = 1.f } });
	Renderer renderer("res/shader_texture.glsl", "res/shader_dynamic.glsl", &engine);

	World world(engine, "res/map.txt");

	Sprite* dsprite = world.PutSprite("res/test2.bmp", 2, 500);
	Dynamic* dynamic = world.CreateDynamic({ 0.f, 0.f }, { 0.f, 0.f }, 100.f, { {"a", dsprite} }, "a");
	Camera cam(dynamic);




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
		world.Draw(renderer, cam, dynamic);

		renderer.Render();
	}

	// clean up
	end(engine);
	return 0;
}