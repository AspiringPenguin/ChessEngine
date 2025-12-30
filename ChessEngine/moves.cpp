#include "moves.h"

namespace moves {
	move encodeMove(square from, square to, piece capture, bool promote, bool isCapture, bool special1, bool special2) {
		return from | (to << 6) | (capture << 12) | (promote << 16) | (isCapture << 17) | (special1 << 18) | (special2 << 19);
	}

	square getFrom(const move& m){
		return square(m & 0x3F);
	}

	square getTo(const move& m){
		return square((m & 0xFC0) >> 6);
	}

	piece getCapture(const move& m){
		return piece((m & 0xF000) >> 12);
	}

	bool getPromoteFlag(const move& m){
		return (m & 0x10000) >> 13;
	}

	piece getPromote(const move& m, const color& color) {
		bool flag1 = getSpecial1(m);
		bool flag2 = getSpecial2(m);

		return piece(flag1 ? (flag2 ? wQueen : wRook) : (flag2 ? wBishop : wKnight) | (color << 3));
	}

	bool getCaptureFlag(const move& m){
		return (m & 0x20000) >> 14;
	}

	bool getSpecial1(const move& m){
		return (m & 0x40000) >> 15;
	}

	bool getSpecial2(const move& m){
		return (m & 0x80000) >> 16;
	}
}