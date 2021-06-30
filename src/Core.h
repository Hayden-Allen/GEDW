#pragma once

namespace engine
{
	constexpr static uint s_VerticesPerQuad = 4, s_IndicesPerQuad = 6;
	constexpr static uint s_IndexOffsets[s_IndicesPerQuad] = { 0, 1, 2, 0, 2, 3 };
}
