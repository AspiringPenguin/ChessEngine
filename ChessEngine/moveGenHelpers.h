#pragma once
#include "core.h"
#include <array>

namespace moveGen {
	std::array<U64, 64> generateKnightLookup();

	static std::array<U64, 64> knightLookup = generateKnightLookup();
}