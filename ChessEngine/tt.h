#pragma once
#include "core.h"
#include <tuple>

namespace tt {
	enum ttResult {
		ttScore, ttMove, ttNull
	};

	enum ttEntryType : char {
		exact, lowerBound, upperBound
	};

	#pragma pack(push)
	#pragma pack(1)
	struct ttEntry {
		move m;
		short score;
		ttEntryType type;
		unsigned char depth;
	};
	#pragma pack(pop)

	std::tuple<ttResult, int> ttProbe(const U64& zobrist, const int& alpha, const int& beta, const int& depth);

	void ttStore(const int& eval, const move& move, const unsigned char& depth);
}