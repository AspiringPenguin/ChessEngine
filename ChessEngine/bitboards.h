#pragma once
#include "core.h"

//For helper functions with bitboards

namespace bitboards {
	void showBitboard(U64 bb);

	constexpr U64 notAFile = ~0x0101010101010101;
	constexpr U64 notABFile = ~0x0303030303030303;
	constexpr U64 notGHFile = ~0xC0C0C0C0C0C0C0C0;
	constexpr U64 notHFile = ~0x8080808080808080;
}