#include "pch.h"
#include "graphics/Sprite.h"
#include "graphics/Renderer.h"
#include "world/dynamic/Dynamic.h"
#include "world/Camera.h"
#include "world/World.h"
#include "script/Script.h"
#include "world/dynamic/Character.h"

using namespace engine;

int main()
{
	EngineInstance engine = init(800, 600, "ACM Game Engine Dev Workshop Series", { .resizable = true, .pixelSize = 2.f, .clear = {.b = 1.f } });
	Renderer renderer("res/shader_texture.glsl", "res/shader_dynamic.glsl", &engine);

	World world(&engine, "res/map.txt");


	Sprite* dsprite = world.PutSprite("res/test2.bmp", 2, 500);
	Character* player = world.CreateCharacter("res/scripts/player.script", { 0.f, 0.f }, { 0.f, 0.f }, 100.f, { {"a", dsprite} }, "a");
	Camera cam("res/scripts/camera.script", { 0.f, 0.f }, { 0.f, 0.f }, 100.f, player);


	while (engine.IsRunning())
	{
		renderer.Clear();

		world.Draw(renderer, cam, player);

		renderer.Render();
	}


	end(engine);
	return 0;
}