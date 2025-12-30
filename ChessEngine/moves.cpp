#include "moves.h"

namespace moves {
	move encodeMove(square from, square to, piece capture, bool promote, bool isCapture, bool special1, bool special2) {
		return from | (to << 6) | (capture << 12) | (promote << 16) | (isCapture << 17) | (special1 << 18) | (special2 << 19);
	}

	square getFrom(move m){
		return square(m & 0x3F);
	}

	square getTo(move m){
		return square((m & 0xFC0) >> 6);
	}

	piece getCapture(move m){
		return piece((m & 0xF000) >> 12);
	}

	bool getPromote(move m){
		return false;
	}

	bool getCaptureFlag(move m){
		return false;
	}

	bool getSpecial1(move m){
		return false;
	}

	bool getSpecial2(move m){
		return false;
	}
}