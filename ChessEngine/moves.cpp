#include "core.h"
#include "moves.h"
#include <iostream>

namespace moves {
	// A base function not designed for use elsewhere
	// enPassant and doublePush are used to encode promotion, as follows:
	// 1 1 queen
	// 1 0 rook
	// 0 1 bishop
	// 0 0 knight
	// Castling are encoded as per UCI with e1g1, e1c1, e8g8, e8c8
	// Castling rights changes are true if a move takes them away

	move encodeMove(square from, square to, piece p, piece capture, bool promote, bool enPassant, bool doublePush, bool wkc, bool wqc, bool bkc, bool bqc) {
		return from | (to << 6) | (p << 12) | (capture << 16) | (promote << 20) | (enPassant << 21) | (doublePush << 22) | (wkc << 23) | (wqc << 24) | (bkc << 25) | (bqc << 26);
	}

	move encodeNormal(square from, square to, piece p, piece capture, bool enPassant, bool doublePush, bool wkc, bool wqc, bool bkc, bool bqc) {
		return encodeMove(from, to, p, capture, false, enPassant, doublePush, wkc, wqc, bkc, bqc);
	}

	move encodePromote(square from, square to, piece p, piece capture, piece promote, bool wkc, bool wqc, bool bkc, bool bqc) {
		bool special1 = (promote == wQueen || promote == bQueen || promote == wRook || promote == bRook);
		bool special2 = (promote == wQueen || promote == bQueen || promote == wBishop || promote == bBishop);

		return encodeMove(from, to, p, capture, true, special1, special2, wkc, wqc, bkc, bqc);
	}

	move encodeCastle(color side, bool queenside, bool wkc, bool wqc, bool bkc, bool bqc) {
		if (side == white) {
			if (!queenside) {
				return encodeMove(E1, G1, wKing, nullPiece, false, false, false, wkc, wqc, false, false);
			}
			else {
				return encodeMove(E1, C1, wKing, nullPiece, false, false, false, wkc, wqc, false, false);
			}
		}
		else {
			if (!queenside) {
				return encodeMove(E8, G8, bKing, nullPiece, false, false, false, false, false, bkc, bqc);
			}
			else {
				return encodeMove(E8, C8, bKing, nullPiece, false, false, false, false, false, bkc, bqc);
			}

		}
	}

	square getFrom(const move& m){
		return square(m & 0x3F);
	}

	square getTo(const move& m){
		return square((m & 0xFC0) >> 6);
	}

	piece getPiece(const move& m) {
		return piece((m & 0xF000) >> 12);
	}

	piece getCapture(const move& m){
		return piece((m & 0xF0000) >> 16);
	}

	//EnPassant
	bool getSpecial1(const move& m){
		return (m & 0x200000) >> 21;
	}

	//Double push
	bool getSpecial2(const move& m){
		return (m & 0x400000) >> 22;
	}

	bool getPromoteFlag(const move& m){
		return (m & 0x100000) >> 20;
	}

	piece getPromote(const move& m, const color& color) {
		bool flag1 = getSpecial1(m);
		bool flag2 = getSpecial2(m);

		return piece((flag1 ? (flag2 ? wQueen : wRook) : (flag2 ? wBishop : wKnight)) | (color << 3));
	}

	bool isEnPassant(const move& m) {
		return getSpecial1(m) && !getPromoteFlag(m);
	}

	bool isDoublePush(const move& m) {
		return getSpecial2(m) && !getPromoteFlag(m);
	}

	std::tuple<bool, bool, bool, bool> getCastleChanges(const move& m) {
		return { (m & 0x800000) >> 23, (m & 0x1000000) >> 24, (m & 0x2000000) >> 25, (m & 0x4000000) >> 26 };
	}

	int getMVVLVAScore(const move& m) {
		return (m & 0xFF000) >> 12;
	}

	void debugMove(move m) {
		std::cout << pieceNames[getPiece(m)] << " " << squareNames[getFrom(m)] << squareNames[getTo(m)];
		std::cout << ((getCapture(m) != nullPiece) ? " takes " + pieceNames[getCapture(m)] : "");
		std::cout << ((getPromoteFlag(m) == true) ? (" promotes to " + pieceNames[getPromote(m, white)]) : std::string(" ")) << std::endl;
		std::cout << isEnPassant(m) << " " << isDoublePush(m) << std::endl;
		auto castleRights = getCastleChanges(m);
		std::cout << std::get<0>(castleRights) << std::get<1>(castleRights) << std::get<2>(castleRights) << std::get<3>(castleRights) << std::endl;
	}

	void showMove(move m) {
		std::cout << squareNames[getFrom(m)] << squareNames[getTo(m)];
		std::cout << ((getPromoteFlag(m) == true) ? (" promotes to " + pieceNames[getPromote(m, white)] + " ") : std::string(" "));
	}
}