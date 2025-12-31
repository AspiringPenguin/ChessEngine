#include "moves.h"

namespace moves {
	move encodeMove(square from, square to, piece p, piece capture, bool promote, bool special1, bool special2, bool enPassant, bool wkc, bool wqc, bool bkc, bool bqc) {
		return from | (to << 6) | (p << 12) | (capture << 16) | (promote << 20) | (special1 << 21) | (special2 << 22) | (enPassant << 23) | (wkc << 24) | (wqc << 25) | (bkc << 26) | (bqc << 27);
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

	piece getPiece(const move& m)
	{
		return piece((m & 0xF000) >> 12);
	}

	piece getCapture(const move& m){
		return piece((m & 0xF0000) >> 16);
	}

	bool getPromoteFlag(const move& m){
		return (m & 0x100000) >> 20;
	}

	piece getPromote(const move& m, const color& color) {
		bool flag1 = getSpecial1(m);
		bool flag2 = getSpecial2(m);

		return piece(flag1 ? (flag2 ? wQueen : wRook) : (flag2 ? wBishop : wKnight) | (color << 3));
	}

	bool getSpecial1(const move& m){
		return (m & 0x200000) >> 21;
	}

	bool getSpecial2(const move& m){
		return (m & 0x400000) >> 22;
	}

	bool getEnPassant(const move& m) {
		return (m & 0x800000) >> 23;
	}

	std::tuple<bool, bool, bool, bool> getCastleChanges(const move& m) {
		return { (m & 0x1000000) >> 24, (m & 0x2000000) >> 25, (m & 0x4000000) >> 26, (m & 0x8000000) >> 27 };
	}
}