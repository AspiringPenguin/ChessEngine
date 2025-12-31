#include "moves.h"

namespace moves {
	move encodeMove(square from, square to, piece capture, bool promote, bool special1, bool special2, bool enPassant, bool wkc, bool wqc, bool bkc, bool bqc) {
		return from | (to << 6) | (capture << 12) | (promote << 16) | (special1 << 17) | (special2 << 18) | (enPassant << 19) | (wkc << 20) | (wqc << 21) | (bkc << 22) | (bqc << 23);
	}

	//Special 1 = double pawn push
	//Special 2 = king castle
	//Special 1 and 2 = queen castle

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

	bool getSpecial1(const move& m){
		return (m & 0x20000) >> 17;
	}

	bool getSpecial2(const move& m){
		return (m & 0x40000) >> 18;
	}

	bool getEnPassant(const move& m) {
		return (m & 0x80000) >> 19;
	}

	std::tuple<bool, bool, bool, bool> getCastleChanges(const move& m) {
		return { (m & 0x100000) >> 20, (m & 0x200000) >> 21, (m & 0x400000) >> 22, (m & 0x800000) >> 23 };
	}
}