#pragma once
#include "core.h"
#include <array>

namespace moveGen {
	std::array<U64, 64> generateKnightLookup();
	std::array<U64, 64> generateKingLookup();
	std::array<U64, 64> generateRookPextMasks();
	std::array<U64, 64> generateBishopPextMasks();

	std::array<bool, 64> bitboardToMailbox(U64 bb);

	static std::array<U64, 64> knightLookup = generateKnightLookup();
	static std::array<U64, 64> kingLookup = generateKingLookup();
	static std::array<U64, 64> rookPextMasks = generateRookPextMasks();
	static std::array<U64, 64> bishopPextMasks = generateBishopPextMasks();

	constexpr U64 whiteQueenCastleMask = 0x0E;
	constexpr U64 whiteKingCastleMask = 0x60;

	constexpr U64 blackQueenCastleMask = 0x0E00000000000000;
	constexpr U64 blackKingCastleMask = 0x6000000000000000;
}