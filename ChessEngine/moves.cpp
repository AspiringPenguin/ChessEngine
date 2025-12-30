#include "moves.h"

namespace moves {
	move encodeMove(square from, square to, piece capture, bool promote, bool isCapture, bool special1, bool special2) {
		return from | (to << 6) | (capture << 12) | (promote << 16) | (isCapture << 17) | (special1 << 18) | (special2 << 19);
	}
}