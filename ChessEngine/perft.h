#pragma once
#include "position.h"

namespace perft {
	class PerftSearcher {
		Position p;
		template <color c> int perft(int depthRemaining, bool top);

	public:
		PerftSearcher();
		PerftSearcher(const std::string& fen);

		int go(int depth, bool debug = false);
	};
}