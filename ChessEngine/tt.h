#pragma once
#include "core.h"
#include <tuple>

namespace tt {
	enum ttResult {
		ttScore, ttMove, ttNull
	};

	enum ttEntryType : short {
		exact, lowerBound, upperBound
	};

	#pragma pack(push)
	#pragma pack(2)
	struct ttEntry {
		move m;
		short score;
		ttEntryType type;
	};
	#pragma pack(pop)

	std::tuple<ttResult, int> ttProbe(const U64& zobrist, const int& alpha, const int& beta, const int& depth);

	void ttStore(const int& eval, const move& move, const int& depth); //Only possibly store
}