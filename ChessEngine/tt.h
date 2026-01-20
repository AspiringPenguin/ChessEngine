#pragma once
#include "core.h"
#include <tuple>

namespace tt {
	enum ttResult {
		ttNull = 0,
		ttScore = 1,
		ttMove = 2,
	};

	enum ttEntryType : char {
		exact = 0,
		lowerBound = 1,
		upperBound = 2,
		empty = 3
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

	void setSize(int mb);

	std::tuple<ttResult, int, move> ttProbe(const U64& zobrist, const int& alpha, const int& beta, const int& depth);

	void ttStore(const U64& zobrist, const int& eval, const move& move, const unsigned char& depth, const ttEntryType& entryType, bool firstMove);
}