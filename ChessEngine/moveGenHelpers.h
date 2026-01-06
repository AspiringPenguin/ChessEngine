#pragma once
#include "core.h"
#include <array>

namespace moveGen {
	std::array<U64, 64> generateKnightLookup();
	std::array<U64, 64> generateKingLookup();

	static std::array<U64, 64> knightLookup = generateKnightLookup();
	static std::array<U64, 64> kingLookup = generateKingLookup();
}