#include "zobrist.h"

U64 zobrist::zobristPieces(const std::array<piece, 64>& mailbox) {
	U64 res = 0;
	int n = 0;
	for (int i = -1; const piece& p : mailbox) {
		i++;
		res ^= values[64 * pieceLookup[p] + i];
	}
	return res;
}
