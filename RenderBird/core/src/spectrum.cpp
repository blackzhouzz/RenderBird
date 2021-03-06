#include "spectrum.h"

namespace Core
{
	const RGB32 RGB32::RED		= RGB32(1.0f, 0.0f, 0.0f);
	const RGB32 RGB32::GREEN	= RGB32(0.0f, 1.0f, 0.0f);
	const RGB32 RGB32::BLUE		= RGB32(0.0f, 0.0f, 1.0f);
	const RGB32 RGB32::BLACK	= RGB32(0.0f, 0.0f, 0.0f);
	const RGB32 RGB32::WHITE	= RGB32(1.0f, 1.0f, 1.0f);
	const RGB32 RGB32::YELLOW	= RGB32(1.0f, 1.0f, 0.0f);

	const RGB8 RGB8::RED		= RGB8(255, 0, 0);
	const RGB8 RGB8::GREEN		= RGB8(0, 255, 0);
	const RGB8 RGB8::BLUE		= RGB8(0, 0, 255);
	const RGB8 RGB8::BLACK		= RGB8(0, 0, 0);
	const RGB8 RGB8::WHITE		= RGB8(255, 255, 255);
	const RGB8 RGB8::YELLOW		= RGB8(255, 255, 0);

	const RGBA32 RGBA32::RED	= RGBA32(1.0f, 0.0f, 0.0f, 1.0f);
	const RGBA32 RGBA32::GREEN	= RGBA32(0.0f, 1.0f, 0.0f, 1.0f);
	const RGBA32 RGBA32::BLUE	= RGBA32(0.0f, 0.0f, 1.0f, 1.0f);
	const RGBA32 RGBA32::BLACK	= RGBA32(0.0f, 0.0f, 0.0f, 1.0f);
	const RGBA32 RGBA32::WHITE	= RGBA32(1.0f, 1.0f, 1.0f, 1.0f);
	const RGBA32 RGBA32::YELLOW = RGBA32(1.0f, 1.0f, 0.0f, 1.0f);

	const RGBA8 RGBA8::RED		= RGBA8(255, 0, 0, 255);
	const RGBA8 RGBA8::GREEN	= RGBA8(0, 255, 0, 255);
	const RGBA8 RGBA8::BLUE		= RGBA8(0, 0, 255, 255);
	const RGBA8 RGBA8::BLACK	= RGBA8(0, 0, 0, 255);
	const RGBA8 RGBA8::WHITE	= RGBA8(255, 255, 255, 255);
	const RGBA8 RGBA8::YELLOW	= RGBA8(255, 255, 0, 255);
}