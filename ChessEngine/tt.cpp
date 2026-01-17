#include "tt.h"

namespace tt {
	std::vector<ttEntry> tt = std::vector<ttEntry>();

	U64 size;

	void setSize(int mb){
		size = static_cast<U64>(mb) * 1024 * 1024 / sizeof(ttEntry);

		tt.clear();
		tt.assign(size, ttEntry{ -1, 0, exact, 0 });
	}

	std::tuple<ttResult, int> ttProbe(const U64& zobrist, const int& alpha, const int& beta, const int& depth)
	{
		U64 index = zobrist & (size - 1);
		auto entry = tt[index];
		if (entry.m == -1) { //No stored value
			return { ttNull, -1 };
		}
		if (entry.type == exact || (entry.type == lowerBound && entry.score > beta) || (entry.type == upperBound && entry.score < alpha)) {
			return { ttScore, entry.score };
		}
		return { ttMove, entry.m };
	}

	void ttStore(const U64& zobrist, const int& eval, const move& move, const unsigned char& depth, const ttEntryType& entryType, bool firstMove) {
		U64 index = zobrist & (size - 1);
		if (tt[index].m == -1) { //Is empty
			tt[index] = ttEntry{ move, static_cast<short>(eval), entryType, depth };
		}
		//Always replace (if exact, if is upper bound (no improvements over alpha, all would otherwise be searched) or if best move not ordered first) and depth >= existing entry
		else if (depth >= tt[index].depth) {
			if (entryType != lowerBound || !firstMove) { 
				tt[index] = ttEntry{ move, static_cast<short>(eval), entryType, depth };
			}
		}
	}
}