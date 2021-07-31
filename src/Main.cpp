#include "pch.h"
#include "graphics/Sprite.h"
#include "graphics/Renderer.h"
#include "world/dynamic/Dynamic.h"
#include "world/Camera.h"
#include "world/World.h"
#include "script/Script.h"

using namespace engine;

int main()
{
	EngineInstance engine = init(800, 600, "ACM Game Engine Dev Workshop Series", { .resizable = true, .pixelSize = 2.f, .clear = {.b = 1.f } });
	Renderer renderer("res/shader_texture.glsl", "res/shader_dynamic.glsl", &engine);

	World world(engine, "res/map.txt");


	Script test("res/scripts/test.script");
	std::vector<Scriptable*> env;
	test.Run({ &renderer, &world }, nullptr, env);
	return 0;


	Sprite* dsprite = world.PutSprite("res/test2.bmp", 2, 500);
	Dynamic* dynamic = world.CreateDynamic({ 0.f, 0.f }, { 0.f, 0.f }, 100.f, { {"a", dsprite} }, "a");
	Camera cam(dynamic);


	while (engine.IsRunning())
	{
		renderer.Clear();

		math::Vec2<float> offset =
		{
			1.f * engine.IsKeyPressed('A') - engine.IsKeyPressed('D'),
			1.f * engine.IsKeyPressed('S') - engine.IsKeyPressed('W')
		};
		if (!math::isZero(offset.x) && !math::isZero(offset.y))
			offset /= math::sqrt(2.f);
		dynamic->SetVel(offset * -dynamic->GetSpeed());
		world.Draw(renderer, cam, dynamic);

		renderer.Render();
	}


	end(engine);
	return 0;
}