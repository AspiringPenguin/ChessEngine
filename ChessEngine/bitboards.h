#pragma once
#include "core.h"

//For helper functions with bitboards

namespace bitboards {
	void showBitboard(U64 bb);

	constexpr U64 notAFile = ~0x0101010101010101;
	constexpr U64 notABFile = ~0x0303030303030303;
	constexpr U64 notGHFile = ~0xC0C0C0C0C0C0C0C0;
	constexpr U64 notHFile = ~0x8080808080808080;

	constexpr U64 rank1 = 0x00000000000000FF;
	constexpr U64 rank2 = 0x000000000000FF00;
	constexpr U64 rank3 = 0x0000000000FF0000;
	constexpr U64 rank4 = 0x00000000FF000000;
	constexpr U64 rank5 = 0x000000FF00000000;
	constexpr U64 rank6 = 0x0000FF0000000000;
	constexpr U64 rank7 = 0x00FF000000000000;
	constexpr U64 rank8 = 0xFF00000000000000;
}