#pragma once
#include "core.h"

//For helper functions with bitboards

namespace bitboards {
	void showBitboard(U64& bb);

	constexpr U64 notAFile = ~0x0101010101010101;
	constexpr U64 notHFile = ~0x8080808080808080;
}