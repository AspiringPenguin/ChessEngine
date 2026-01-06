#include "moveGenHelpers.h"
#include "bitboards.h"

namespace moveGen {
	std::array<U64, 64> generateKnightLookup() {
		auto bitboards = std::array<U64, 64>();

		U64 bb = 0;

		for (int i = 0; i < 64; i++) {
			bb = (1ull << i);

			//From https://www.chessprogramming.org/Knight_Pattern

			U64 l1 = (bb >> 1) & bitboards::notHFile;
			U64 l2 = (bb >> 2) & bitboards::notGHFile;
			U64 r1 = (bb << 1) & bitboards::notAFile;
			U64 r2 = (bb << 2) & bitboards::notABFile;
			U64 h1 = l1 | r1;
			U64 h2 = l2 | r2;

			bitboards[i] = (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
		}

		return bitboards;
	}
}